-module('2018111010_2').
% -module(multi_rpc_test).
-export([main/1]). 

read_three_ints(InpDevice) ->
    case io:fread(InpDevice,"kjds", "~d ~d ~d") of
        {ok, [A, B, C]} -> [A, B, C];
        _          -> []
    end.

read_two_ints(InpDevice) ->
    case io:fread(InpDevice,"kjds", "~d ~d") of
        {ok, [A, B]} -> [A, B];
        _          -> []
    end.

read_int(InpDevice) ->
    case io:fread(InpDevice,"kjds", "~d") of
        {ok, [A]} -> [A];
        _          -> []
    end.

point_update(Lst, Loc, Val) ->
    lists:sublist(Lst, Loc - 1) ++ [ Val ] ++ lists:nthtail(Loc, Lst).

update(End,End,_,Distances)->
    Distances;
update(Start,End,EdgeList,Distances)->
    [VertA,VertB,Weight]= lists:nth(Start+1,EdgeList),
    Dist1 = point_update(Distances, VertA, min(lists:nth(VertA,Distances),Weight + lists:nth(VertB,Distances))),
    NewDist = point_update(Dist1, VertB, min(lists:nth(VertB,Distances),Weight + lists:nth(VertA,Distances))),
    update(Start+1, End, EdgeList, NewDist).

proc(I,AvgEdges,EdgeCount,EdgeList,Distances,Parent) ->
    Start = I*AvgEdges,
    End = min(Start+AvgEdges-1,EdgeCount-1),
    if 
    Start>End ->
        Parent ! {self(), Distances};
    true ->
        NewDist = update(Start,End+1,EdgeList,Distances),
        % io:format("~p~p~n", [Start,End]),
        % io:format("~p~n", [Parent]),
        Parent ! {self(), NewDist}
    end.

minim([],[])->
    [];
minim([EleA|ListA],[EleB|ListB]) ->
    [min(EleA, EleB)|minim(ListA,ListB)].

% temp(A,B) ->
%     A.

update_distances(Distances,[]) -> 
    Distances;
update_distances(Distances,[Pid|Pids]) ->
    % io:format("~p~n", [Pid]),
    receive
        {Pid, Result} ->
            io:format("", [])
    end,
    UpdatedDist = update_distances(Distances,Pids),
    % io:format("~p~p~n", [UpdatedDist,Result]),
    minim(Result,UpdatedDist).
    % UpdatedDist.

bf(Distances,_,_,_,0) ->
    Distances;
bf(Distances,EdgeList,EdgeCount,ProcCount,Times) ->
    AvgEdges = (EdgeCount + ProcCount - 1) div ProcCount,
    CPID = self(),
    Processes = [spawn(fun() -> proc(I,AvgEdges,EdgeCount,EdgeList,Distances,CPID) end) || I <- lists:seq(0, ProcCount-1)],
    % io:format("~p~n", [Processes]),
    NewDist = update_distances(Distances, Processes),
    % NewDist = temp(Distances, Processes),
    bf(NewDist,EdgeList,EdgeCount,ProcCount,Times-1).

printer([],_,_) ->
    true;
printer([Val | Rest], Ind, OutputFile) ->
    {ok, Output} = file:open(OutputFile, [append]),
    io:format(Output, "~p ~p~n", [Ind, Val]),
    printer(Rest,Ind+1,OutputFile).

main(Args) -> 
    [InputFile, OutputFile] = Args,
    {ok, Input} = file:open(InputFile, [read]),
    {ok, Output} = file:open(OutputFile, [write]),
    io:fwrite(Output,"",[]),

    [ProcCount] = read_int(Input),
    [NodeCount, EdgeCount] = read_two_ints(Input),
    EdgeList = [read_three_ints(Input) || _ <- lists:seq(0, EdgeCount-1)],
    [StartVertex] = read_int(Input),

    % io:format("~p~p~p~p~n", [ProcCount, NodeCount, EdgeCount, StartVertex]),
    % io:format("~p~n", [EdgeList]),

    % Initialize distance matrix
    Distances = [100000000000 || _<-lists:seq(1,NodeCount)],
    NewDist = point_update(Distances, StartVertex, 0),

    % io:format("~p~n", [self()]),

    % Start the Bellman-Ford algo
    FinalDistances = bf(NewDist,EdgeList,EdgeCount,ProcCount,NodeCount),
    % io:format("~p~n", [FinalDistances]),
    printer(FinalDistances, 1, OutputFile),

    file:close(Input),
    file:close(Output)
    .
