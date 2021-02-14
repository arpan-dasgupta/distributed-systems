-module(demo). 
-export([start/1]). 

read(File) ->
    case file:read_line(File) of
        {ok, Data} -> Data;
        eof        -> []
    end.

read_two_ints(InpDevice) ->
    case io:fread(InpDevice,"kjds", "~d ~d") of
        {ok, [A, B]} -> [A, B];
        _          -> []
    end.

start(Args) -> 
    io:format("Args: ~p\n", [Args]),
    [InputFile, OutputFile] = Args,
    {ok, Input} = file:open(InputFile, [read]),
    {ok, Output} = file:open(OutputFile, [write]),
    % Data = read(Input),
    % NewData = lists:map(fun(X) -> {Int, _} = string:to_integer(X), 
    %                 Int end, 
    %       string:tokens(Data, " ")),
    % [A,B] = string:tokens(Data, " "),
    % NewData = string:tokens(Data, " "),
    % io:format("Device: ~w\n", [Input]),
    [A,B] = read_two_ints(Input),
    % NewData = string:tokens(Data," "),
    % AA = string:to_integer(A),
    % AA = list_to_integer(A),
    io:fwrite("~w",[A]),
    io:fwrite("~w",[B]),
    % io:fwrite(B),
    file:close(Input).
    % io:fwrite(NewData).