//
//  Copyright 2012 Alin Dobra and Christopher Jermaine
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
#include "GLAWayPointImp.h"
#include "CPUWorkerPool.h"
#include "Logging.h"
#include "Constants.h"

#include <iostream>

GLAWayPointImp :: GLAWayPointImp () {
    PDEBUG ("GLAWayPointImp :: GLAWayPointImp ()");
    lastFragmentId = 0; // start search from beginning
}

GLAWayPointImp :: ~GLAWayPointImp () {
    PDEBUG ("GLAWayPointImp :: ~GLAWayPointImp ()");
}

void GLAWayPointImp :: TypeSpecificConfigure (WayPointConfigureData &configData) {
    PDEBUG ("GLAWayPointImp :: TypeSpecificConfigure ()");
    // load up the list where we will place done queries
    GLAConfigureData tempConfig;
    tempConfig.swap (configData);

    QueryIDToBool& retStates = tempConfig.get_resultIsState();
    resultIsState.SuckUp(retStates);

    // tell people that we are ready to go with our queries... these messages will
    // eventually make it down to the table scan, which will begin producing data
    QueryExitContainer queries;
    GetFlowThruQueryExits(queries);

    FOREACH_TWL(iter, queries){
        // start the pool of states for the new query
        QueryID q=iter.query;
        GLAStateContainer cont; // empty container
        if (!myQueryToGLAStates.IsThere(q)){
            myQueryToGLAStates.Insert(q, cont);
        }
    } END_FOREACH

    tempConfig.swap(configData);

    GPWayPointImp::Configure(configData);
}

bool GLAWayPointImp::PreProcessingPossible( CPUWorkToken& token ) {
    if( queriesToPreprocess.IsEmpty() )
        return false;

    QueryIDSet curQueries = queriesToPreprocess;
    // Clear out queriesToPreprocess
    queriesToPreprocess.Difference(curQueries);

    HistoryList lineage;
    GLAHistory hist(GetID(), -1);
    lineage.Insert(hist);

    QueryExitContainer qExits;

    while( !curQueries.IsEmpty() ) {
        QueryID temp = curQueries.GetFirst();

        QueryExit qe = GetExit( temp );
        qExits.Append(qe);
    }

    QueryExitContainer whichOnes;
    whichOnes.copy(qExits);

    GLAPreProcessWD workDesc( qExits );

    WayPointID myID = GetID();
    WorkFunc myFunc = GetWorkFunction( GLAPreProcessWorkFunc :: type );

    myCPUWorkers.DoSomeWork( myID, lineage, whichOnes, token, workDesc, myFunc );

    return true;
}

bool GLAWayPointImp::PostProcessingPossible( CPUWorkToken& token ) {
    PDEBUG ("GLAWayPointImp :: PostProcessingPossible()");
    if( queriesMerging.IsEmpty() )
        return false;

    QueryIDSet iter = queriesMerging;

    QueryToGLASContMap stateM;
    QueryExitContainer whichOnes;

    while( !iter.IsEmpty() ) {
        // find the states for this query
        QueryID q=iter.GetFirst();
        QueryExit qe = GetExit(q);
        QueryID foo = q;
        FATALIF(!myQueryToGLAStates.IsThere(q), "Why I am having a queryToComplete but no GLA state container?");

        GLAStateContainer& cont = myQueryToGLAStates.Find(q);
        int& mCount = mergeInProgress.Find(qe.query).GetData();

        //schedule merge only when we have 2 or more states in the GLAStateContainer
        // the exception is the case when we only have 1 state from the beginning
        if((cont.Length() > 1) || (cont.Length() == 1 && mCount == 0)) {
            //              cout<<"cont Length before merge schedule: "<<cont.Length()<<endl;
            GLAStateContainer tempCont;
            int mergeCount=0;
            for(cont.MoveToStart(); cont.RightLength();){
                GLAState cs;
                cont.Remove(cs);
                tempCont.Insert(cs);
                mergeCount++;

                //check if merge limit is reached
                if(mergeCount == MERGE_AT_A_TIME){
                    break;
                }
            }
            //cout<<"length of cont in merge for query"<<q.GetStr()<<" = "<<tempCont.Length()<<endl;
            stateM.Insert(q,tempCont);
            QueryExit qe = GetExit(foo);
            whichOnes.Insert(qe);

            //set the mergeInProgress for the query
            mCount++;
            //    cout<<"mCount "<<mCount<<endl;
        }

    }

    //check if we have anything to do to make use of the granted token
    stateM.MoveToStart();
    if(!stateM.AtEnd()){
        QueryExitContainer whichOnes1;
        whichOnes1.copy (whichOnes);

        //dummy fragmentNo for merge
        GLAHistory hist (GetID (), -1);
        HistoryList lineage;
        lineage.Insert(hist);

        // now, actually get the work done!
        GLAMergeStatesWD workDesc (whichOnes1, stateM);

        WayPointID myID = GetID();
        WorkFunc myFunc = GetWorkFunction( GLAMergeStatesWorkFunc :: type );

        myCPUWorkers.DoSomeWork( myID, lineage, whichOnes, token, workDesc, myFunc );

        return true;
    }
    else{
        return false;
    }

}

bool GLAWayPointImp :: PreFinalizePossible( CPUWorkToken& token ) {
    PDEBUG ("GLAWayPointImp :: FinalizePossible()");

    if( queriesCounting.IsEmpty() )
        return false;

    QueryIDSet qryOut = queriesCounting;
    queriesCounting.Difference(qryOut);

    QueryExitContainer whichOnes;

    QueryIDSet qryIter = qryOut;
    while( !qryIter.IsEmpty() ) {
        QueryID curID = qryIter.GetFirst();
        QueryExit qe = GetExit(curID);
        whichOnes.Append(qe);
    }

    QueryToGLAStateMap tempMergedStates;
    tempMergedStates.copy(mergedStates);

    QueryExitContainer whichOnes1;
    whichOnes1.copy(whichOnes);

    QueryToGLASContMap curConstStates = GetConstStates();

    GLAPreFinalizeWD workDesc (whichOnes1, tempMergedStates, curConstStates);

    GLAHistory hist (GetID (), 0);
    HistoryList lineage;
    lineage.Insert(hist);

    WayPointID myID = GetID();
    WorkFunc myFunc = GetWorkFunction( GLAPreFinalizeWorkFunc::type );

    myCPUWorkers.DoSomeWork( myID, lineage, whichOnes, token, workDesc, myFunc );

    return true;
}

bool GLAWayPointImp::FinalizePossible( CPUWorkToken& token ) {
    PDEBUG ("GLAWayPointImp :: FinalizePossible()");

    if( queriesFinalizing.IsEmpty() )
        return false;

    int fragmentNo;
    lastFragmentId = queryFragmentMap.FindFirstSet(lastFragmentId);
    if (lastFragmentId == -1){
        lastFragmentId = 0; // start from the beginning next time
        return false; // we did not find any fragment
    } else {
        fragmentNo = lastFragmentId;
    }

    //whichOnes will contains only one query

    Bitstring qryOut = queryFragmentMap.GetBits(fragmentNo);
    qryOut = qryOut.GetFirst();
    queryFragmentMap.Clear(fragmentNo, qryOut); // queries out

    QueryExit whichOne = GetExit(qryOut);

    QueryExitContainer whichOnes;
    QueryExit whichOneCopy = whichOne;
    whichOnes.Append(whichOneCopy);

    GLAState& myState = mergedStates.Find( qryOut );
    GLAState stateCopy;
    stateCopy.copy(myState);

    // this cleans up mergedStates
    GLAFinalizeWD workDesc (fragmentNo, whichOne, stateCopy);

    // we now create a history list data object...
    GLAHistory hist (GetID (), fragmentNo);
    HistoryList lineage;
    lineage.Insert(hist);

    WayPointID myID = GetID();
    WorkFunc myFunc;

    bool rezState = resultIsState.Find(qryOut).GetData();

    if( rezState )
        myFunc = GetWorkFunction( GLAFinalizeStateWorkFunc::type );
    else
        myFunc = GetWorkFunction( GLAFinalizeWorkFunc::type );

    myCPUWorkers.DoSomeWork( myID, lineage, whichOnes, token, workDesc, myFunc );

    return true;
}

#if 0
bool GLAWayPointImp::MergeDone() {
    PDEBUG ("GLAWayPointImp :: MergeDone()");
    //check if we have every qid in mergedStates
    FOREACH_EM(qid, cont, myQueryToGLAStates){
      if(!mergedStates.IsThere(qid)){
        return false;
      }
    } END_FOREACH;

    //here, means Merge is done
    return true;
}
#endif

bool GLAWayPointImp :: PreProcessingComplete( QueryExitContainer& whichOnes, HistoryList& history, ExecEngineData& data) {
    PDEBUG("GLAWayPointImp :: PreProcessingComplete()");

    GLAPreProcessRez temp;
    temp.swap(data);

    QueryExitContainer endAtMe;
    GetEndingQueryExits(endAtMe);

    // Any const states that were generated.
    QueryToGLASContMap& rezConstStates = temp.get_constStates();

    QueryExitContainer startProcessing;
    QueryIDSet startTerminating;

    FOREACH_TWL( curQuery, whichOnes ) {
        QueryID curID = curQuery.query;

        int curStatesNeeded = NumStatesNeeded( curID );

        if( curStatesNeeded == 0 ) {
            QueryExit curQueryCopy = curQuery;
            startProcessing.Append(curQueryCopy);
        } else {
            startTerminating.Union(curQuery.query);
        }
    }END_FOREACH;

    AddGeneratedStates( rezConstStates );

    FOREACH_TWL( curQuery, startProcessing ) {
        queriesProcessing.Union(curQuery.query);

        QueryID key;
        HoppingUpstreamMsg value;
        cachedProducingMessages.Remove(curQuery.query, key, value);

        SendHoppingUpstreamMsg( value );
    }END_FOREACH;

    // If we have any queries that still require states, send the start producing
    // messages to the terminating query exits.
    FOREACH_TWL(qe, endAtMe) {
        if( startTerminating.Overlaps(qe.query) ) {
            SendStartProducingMsg(qe);
        }
    } END_FOREACH;

    return true;
}

bool GLAWayPointImp :: ProcessChunkComplete( QueryExitContainer& whichOnes, HistoryList& history, ExecEngineData& data ) {
    PDEBUG ("GLAWayPointImp :: ProcessChunkComplete()");
    // extract the states comming back
    GLAStatesRez temp;
    temp.swap(data);

    FOREACH_EM(key, d, temp.get_glaStates()){
#ifdef DEBUG
        cout<<"\nStateChunk "<<key.GetStr()<<" "<<d.get_glaType()<<endl;
#endif
        FATALIF(!myQueryToGLAStates.IsThere(key), "Did not find the entry ");
        GLAStateContainer& cont = myQueryToGLAStates.Find(key);
        cont.Insert(d);
    }END_FOREACH;

    return true;
}

bool GLAWayPointImp :: PostProcessingComplete( QueryExitContainer& whichOnes, HistoryList& history, ExecEngineData& data ) {
    PDEBUG ("GLAWayPointImp :: PostProcessingComplete()");
    //      cout<<"\n"<<GetID().getName()<<" Merged recvd"<<endl;
    GLAStatesRez rez;
    rez.swap(data);

    QueryToGLAStateMap& tempGlaStates = rez.get_glaStates();
    for(tempGlaStates.MoveToStart(); !tempGlaStates.AtEnd();){
        QueryID q = tempGlaStates.CurrentKey();
        int& mCount = mergeInProgress.Find(q).GetData();
        //received one merge, hence decrement the mergedCounter
        mCount--;

        FATALIF(!myQueryToGLAStates.IsThere(q), "Why I am having a returned state but no GLA state container?");
        GLAStateContainer& cont = myQueryToGLAStates.Find(q);
        //            cout<<"cont Length before: "<<cont.Length()<<" mCount: "<<mCount<<endl;

        if(mCount == 0 && cont.Length() == 0){
            //we are done, append this <qid, state> to mergedStates and this query to mergedQueries
            //              cout<<endl<<GetID().getName()<<"Merge done for the query: "<<q.GetStr()<<endl;
            QueryID foo;
            GLAState mystate;
            tempGlaStates.Remove(q, foo, mystate);
            mergedStates.Insert(foo, mystate);

            QueryExit qe = GetExit(q);

            queriesMerging.Difference(q);
            queriesCounting.Union(q);
        }
        else {
            //remove state from map and append to the GLAStateContainer as we are not done yet
            QueryID foo;
            GLAState mystate;
            tempGlaStates.Remove(q, foo, mystate);
            cont.Append(mystate);
            //            cout<<"cont Length after: "<<cont.Length()<<endl;
        }
    }

    return true;
}

bool GLAWayPointImp :: PreFinalizeComplete( QueryExitContainer & whichOnes, HistoryList & history, ExecEngineData& data) {
    PDEBUG("GLAWayPointImp :: PreFinalizeComplete()");
    GLAStatesFrRez rez;
    rez.swap(data);

    QueryIDSet restart = rez.get_queriesToIterate();
    queriesToRestart.Union(restart);

    QueryIDSet finished;

    FOREACH_EM(qid, frags, rez.get_fragInfo()) {
        queriesFinalizing.Union(qid);

        if( frags > 0 ) {
            queryFragmentMap.ORAll( qid, frags );
        }
        else {
            // Skip to end
            finished.Union(qid);
        }
    } END_FOREACH;

    fragmentsLeft.SuckUp(rez.get_fragInfo());

    if( !finished.IsEmpty() ) {
        FinishQueries( finished );
    }

    return true;
}

bool GLAWayPointImp :: FinalizeComplete( QueryExitContainer& whichOnes, HistoryList& history, ExecEngineData& data ) {
    PDEBUG ("GLAWayPointImp :: FinalizeComplete()");
    int frag;

    history.MoveToStart();
    if (history.Current().Type() == GLAHistory::type){
        FATALIF(history.RightLength () != 1, "Why do we have more than the GLA lineage?");
        GLAHistory myHistory;
        myHistory.copy(history.Current());
        frag = myHistory.get_whichFragment();
        //cout<<"Finalized recvd for fragmentNo: "<<frag<<endl;
    }

    return true;
}

void GLAWayPointImp :: GotChunkToProcess ( CPUWorkToken & token, QueryExitContainer& whichOnes, ChunkContainer& chunk, HistoryList& lineage) {
    PDEBUG ("GLAWayPointImp :: GotChunkToProcess ()");

    // now build the work spec
    QueryToGLAStateMap qToGLAState;
    FOREACH_TWL(qe, whichOnes){
        QueryID foo = qe.query;
        FATALIF(!myQueryToGLAStates.IsThere(qe.query), "Did not find the entry ");
        GLAStateContainer& cont = myQueryToGLAStates.Find(qe.query);
        if (cont.Length()>0){
            GLAState state;
            cont.Remove(state);
            qToGLAState.Insert(foo, state);
        } // if not, we put no state out, the generated code will create it
    }END_FOREACH;

    QueryExitContainer whichOnesCopy;
    whichOnesCopy.copy( whichOnes );

    QueryToGLASContMap qToConstState = GetConstStates();

    // Can just pass garbageStates to the constructor, it will be swapped out for an
    // empty map.
    GLAProcessChunkWD workDesc (whichOnes, qToGLAState, qToConstState, chunk.get_myChunk(), garbageStates);

    WorkFunc myFunc = GetWorkFunction( GLAProcessChunkWorkFunc::type);

    WayPointID myID = GetID();
    myCPUWorkers.DoSomeWork (myID, lineage, whichOnesCopy, token, workDesc, myFunc);
}

void GLAWayPointImp :: GotAllStates( QueryID query ) {
    // Got the last state we needed, we'll start processing now.
    queriesProcessing.Union(query);

    QueryID key;
    HoppingUpstreamMsg value;
    cachedProducingMessages.Remove(query, key, value);

    SendHoppingUpstreamMsg( value );
}

bool GLAWayPointImp :: ReceivedQueryDoneMsg( QueryExitContainer& whichOnes ) {
    PDEBUG ("GLAWayPointImp :: ReceivedQueryDoneMsg()");
    // extract the queries that are done, add them to the list of those to complete
    for (whichOnes.MoveToStart (); whichOnes.RightLength (); ) {
        QueryExit myExit;
        whichOnes.Remove (myExit);
        QueryID currentID = myExit.query;
        QueryID qID = myExit.query;
        FATALIF(myExit.query.IsEmpty(), "This should be valid");

        //initialize mergeInProgress for each query and set it to 0
        Swapify<int> val(0);
        mergeInProgress.Insert(qID, val);

        // the insert swaps out qID, so we need to reset it
        qID = currentID;
        queriesProcessing.Difference(qID);

        // Find out how many states we have. If we have only one, we don't
        // need to merge.
        FATALIF( !myQueryToGLAStates.IsThere( qID ), "Why don't we have any states for this query?");
        GLAStateContainer& myStates = myQueryToGLAStates.Find(qID);
        myStates.MoveToStart();

        if( myStates.Length() > 1 ) {
            // More than one state, merge them
            queriesMerging.Union(qID);
        }
        else {
            // Only one state, skip to pre-finalize.
            GLAState singleState;
            myStates.Remove(singleState);

            QueryID key = qID;
            mergedStates.Insert(key, singleState);

            queriesCounting.Union(qID);
        }
    }

    // ask for a worker, if we have not already asked
    if (!queriesCounting.IsEmpty() || !queriesMerging.IsEmpty() )
        return true;
    else
        return false;
}

/*
 * Intercept the start producing message. Schedule the query for pre-processing.
 * The start producing message will be sent after pre-processing is complete or
 * all constant states have been received.
 */
bool GLAWayPointImp :: ReceivedStartProducingMsg( HoppingUpstreamMsg& message, QueryExit& whichOne ) {
    PDEBUG ("GLAWayPointImp :: ReceivedStartProducingMsg()");

    QueryID qID = whichOne.query;

    // Check to see if we are running this query for the first time or if we are
    // being asked to rerun a query.
    if( qID.Overlaps(queriesCompleted) ) { // Rerun query
        // Just refinalize the query.
        queriesCompleted.Difference(qID);
        queriesCounting.Union(qID);
    }
    else { // New query
        // Preprocess the query
        queriesToPreprocess.Union(whichOne.query);
    }

    cachedProducingMessages.Insert(qID, message);

    // return true to generate tokens
    return true;
}

void GLAWayPointImp :: ProcessAckMsg (QueryExitContainer &whichOnes, HistoryList &lineage) {
    PDEBUG ("GLAWayPointImp :: ProcessAckMsg ()");
    // make sure that the HistoryList has one item that is of the right type
    lineage.MoveToStart ();
    FATALIF (lineage.RightLength () != 1 || !CHECK_DATA_TYPE(lineage.Current (), GLAHistory),
        "Got a bad lineage item in an ack to a GLA waypoint!");
    GLAHistory myHistory;
    myHistory.swap(lineage.Current());
    int    frag = myHistory.get_whichFragment();

    Bitstring queries; // queries that are dropped
    Bitstring compQ; // completed queries
    Bitstring restartQ;

    FOREACH_TWL(el, whichOnes){
        queries.Union(el.query);
        FATALIF(!fragmentsLeft.IsThere(el.query), "Received an ack for a query I do not know about");
        int& fCount = fragmentsLeft.Find(el.query).GetData();
        fCount--;
        if (fCount == 0){ // done with this query
            compQ.Union(el.query);
            QueryExit qe=el;
        }
    }END_FOREACH;

    LOG_ENTRY_P(2, "Fragment %d of %s query %s PROCESSED",
                frag, GetID().getName().c_str(), queries.GetStr().c_str());

    if (!compQ.IsEmpty()){
        FinishQueries( compQ );
    }
    // need more tokens to resend the dropped stuff
    GenerateTokenRequests();
}

void GLAWayPointImp :: FinishQueries( QueryIDSet queries ) {
    queriesFinalizing.Difference(queries);
    queriesCompleted.Union(queries);

    // Let's see if we have to send any query done messages
    QueryIDSet noRestart = queries;
    noRestart.Difference(queriesToRestart);
    QueryExitContainer doneQueries;

    while( !noRestart.IsEmpty() ) {
        QueryID cur = noRestart.GetFirst();
        QueryExit qe = GetExit(cur);

        doneQueries.Insert(qe);
    }

    if( doneQueries.Length() > 0 )
        SendQueryDoneMsg( doneQueries );

    // Let's see if we have to restart any queries
    if( queriesToRestart.Overlaps( queries ) ) {
        QueryIDSet temp = queries;
        temp.Intersect(queriesToRestart);

        RestartQueries(temp);
    }
}

void GLAWayPointImp :: RestartQueries( QueryIDSet queries ) {
    FATALIF( !queries.IsSubsetOf(queriesCompleted), "Trying to restart queries that aren't complete!");

    queriesCompleted.Difference(queries);
    queriesProcessing.Union(queries);
    queriesToRestart.Difference(queries);

    QueryIDSet it = queries;
    while( !it.IsEmpty() ) {
        QueryID cur = it.GetFirst();
        QueryExit qe = GetExit(cur);

        // Garbage collect the old state.
        QueryID key;
        GLAState value;
        mergedStates.Remove(cur, key, value);
        garbageStates.Insert(key, value);

        SendStartProducingMsg( qe );
    }
}

void GLAWayPointImp :: ProcessDropMsg (QueryExitContainer &whichOnes, HistoryList &lineage) {
    PDEBUG ("GLAWayPointImp :: ProcessDropMsg ()");

    // make sure that the HistoryList has one item that is of the right type
    lineage.MoveToStart ();
    FATALIF (lineage.RightLength () != 1 || !CHECK_DATA_TYPE (lineage.Current (), GLAHistory),
        "Got a bad lineage item in a drop sent to a GLA waypoint!");

    GLAHistory myHistory;
    myHistory.swap(lineage.Current());
    int    frag = myHistory.get_whichFragment();

    cout<<"Drop for fragment: "<<frag<<endl;
    Bitstring queries; // queries that are dropped
    FOREACH_TWL(el, whichOnes){
        queries.Union(el.query);
    }END_FOREACH;

    queryFragmentMap.OROne(frag, queries);
    queriesFinalizing.Union(queries);

    LOG_ENTRY_P(2, "Fragment %d of %s query %s DROPPED",
                frag, GetID().getName().c_str(), queries.GetStr().c_str());


    // need more tokens to resend the dropped stuff
    GenerateTokenRequests();
}
