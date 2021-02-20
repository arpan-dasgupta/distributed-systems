-module('2018111010_1').
% -module(multi_rpc_test).
-export([main/1, rnode/2]). 

read_two_ints(InpDevice) ->
    case io:fread(InpDevice,"kjds", "~d ~d") of
        {ok, [A, B]} -> [A, B];
        _          -> []
    end.

rnode(0, OutputFile) ->
    % io:format("process created: ~w~n", [999] ),
    receive
        {Token,  [H | T]} -> 
            % io:format("Process started, process 0 has token ~p~n",[Token]),
            % io:format("Process started, process 0 has token ~p~n",[H]),
            % io:format("Process started, process 0 has token ~p~n",[T]),
            H ! {0, Token,  T},
            rnode(0, OutputFile);
        {From, Token,  []} -> 
            {ok, Output} = file:open(OutputFile, [append]),
            io:format(Output, "Process 0 received token ~w from process ~w.~n",[Token, From])
            % io:format( "Process 0 received token ~w from process ~w~n",[Token, From])
    end;
rnode(NodeNo, OutputFile) ->
    % io:format("process created: ~w~n", [NodeNo] ),
    receive
        {From, Token,  [H|T]} ->
            {ok, Output} = file:open(OutputFile, [append]),
            io:format(Output, "Process ~w received token ~w from process ~w.~n",[NodeNo, Token, From]),
            % io:format( "Process ~w has received token ~w from process ~w~n",[NodeNo, Token, From]),
            % io:format("Process started, process 0 has token ~p~n",[T]),
            H ! {NodeNo, Token,  T}
    end.

main(Args) -> 
    [InputFile, OutputFile] = Args,
    {ok, Input} = file:open(InputFile, [read]),
    {ok, Output} = file:open(OutputFile, [write]),
    [NodeCount, Token] = read_two_ints(Input),
    % io:fwrite("~w",[NodeCount]),
    % io:fwrite("~w",[Token]),

    % spawn('2018111010_1',node,[0,Token]),
    Processes = [spawn(fun() -> rnode(I,OutputFile) end) || I <- lists:seq(0, NodeCount-1)],
    % [H | _] = Processes,
    io:fwrite(Output,"",[]),
    % io:fwrite(Output,"",[]),

    [Start | T ] = Processes,
    % io:fwrite("~w",[Start]),
    % AllH = [ T | Start],
    AllH = lists:append(T,[Start]),
    % io:format("~p~n", [AllH]),
    
    Start ! {Token, AllH},
    % L = for(1, NodeCount-1),
    % io:format("~p~n", [H]),
    file:close(Input),
    file:close(Output)
    .
