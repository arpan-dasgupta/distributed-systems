-module('2018111010_1').
% -module(multi_rpc_test).
-export([main/1, ring_node/1]). 

read_two_ints(InpDevice) ->
    case io:fread(InpDevice,"kjds", "~d ~d") of
        {ok, [A, B]} -> [A, B];
        _          -> []
    end.

% execute(Args) ->
%     [NodeCount,Token] = read_two_ints(Input),
%     io:fwrite("~w",[NodeCount]),
%     io:fwrite("~w",[Token]),
loop(0) ->
    ok;
loop(X) ->
    io:format("~p processing ~p\n", [self(), X]),
    loop(X-1).


% node(NodeNo, Token, NodeCount) ->
%     ;
% node(NodeNo, NodeCount) ->
%     .

ring_node(I) ->
    io:format("process created: ~w~n", [I] ).

for (N, N) ->
    [spawn('2018111010_1',ring_node,[N])];   
for (I, N) ->
    [spawn('2018111010_1',ring_node,[I])| for(I+1, N)].

main(Args) -> 
    [InputFile, OutputFile] = Args,
    {ok, Input} = file:open(InputFile, [read]),
    {ok, Output} = file:open(OutputFile, [write]),
    [NodeCount,Token] = read_two_ints(Input),
    % io:fwrite("~w",[NodeCount]),
    % io:fwrite("~w",[Token]),

    % spawn('2018111010_1',node,[0,Token]),
    Processes = [spawn(fun() -> loop(1) end) || _ <- lists:seq(1, NodeCount)],
    [H | _] = Processes,
    % L = for(1, NodeCount-1),
    % io:format("~p~n", [L]),
    file:close(Input).
