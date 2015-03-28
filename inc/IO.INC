{$IFDEF DCC}
function AddChar(C: AnsiChar; const S: AnsiString; N: Integer): AnsiString;
	var
	l: Integer;

	begin
	Result:= S;
	l:= Length(Result);
	if l < N then
		Result:= StringOfChar(C, N - l) + Result;
	end;

function AddCharR(C: AnsiChar; const S: AnsiString; N: Integer): AnsiString;
	var
	l : Integer;

	begin
	Result:= S;
	l:= Length(Result);
	if l < N then
		Result:= Result + StringOfChar(C, N - l);
	end;

function Hex2Dec(const S: AnsiString): LongInt;
	var
	HexStr: AnsiString;

	begin
	if Pos(AnsiString('$'), S) = 0 then
		HexStr:= '$' + S
	else
		HexStr:= S;

	Result:= StrToInt(string(HexStr));
	end;

function PadRight(const S: AnsiString; N: Integer): AnsiString; inline;
	begin
	Result:= AddCharR(' ', S, N);
	end;

function PadLeft(const S: AnsiString; N: Integer): AnsiString; inline;
	begin
	Result:= AddChar(' ', S, N);
	end;

function Sscanf(const S: AnsiString; const fmt: AnsiString;
		const Pointers: array of Pointer): Integer;

	var
	i, j, N, m: Integer;
	s1: AnsiString;
	l: LongInt;
	X: Extended;

	function GetInt: Integer;
		begin
		s1:= '';
		while (Length(S) > N) and (S[N] = ' ') do
			Inc(N);

		while (Length(S) >= N) and (S[N] in ['0' .. '9', '+', '-']) do
			begin
			s1:= s1 + S[N];
			Inc(N);
			end;

		Result:= Length(s1);
		end;

	function GetFloat: Integer;
		begin
		s1:= '';

		while (Length(S) > N) and (S[N] = ' ') do
			Inc(N);

		while (Length(S) >= N)
				and (S[N] in ['0' .. '9', '+', '-', '.', 'e', 'E']) do
			begin
			s1:= s1 + S[N];
			Inc(N);
			end;

		Result:= Length(s1);
		end;

	function GetString: Integer;
		begin
		s1:= '';

//dengland Need to check Length first for short-circuit evaluation
		while (Length(S) > N) and (S[N] = ' ') do
			Inc(N);

		while (Length(S) >= N) and (S[N] <> ' ') do
			begin
			s1:= s1 + S[N];
			Inc(N);
			end;

		Result:= Length(s1);
		end;

	function ScanStr(C: AnsiChar): Boolean;
		begin
		while (Length(S) > N) and (S[N] <> C) do
			Inc(N);

		Inc(N);

		if (N <= Length(S)) then
			Result:= True
		else
			Result:= False;
		end;

	function GetFmt: Integer;
		begin
		Result:= -1;

		while (True) do
			begin
			while (fmt[m] = ' ') and (Length(fmt) > m) do
				Inc(m);

			if (m >= Length(fmt)) then
				Break;

			if (fmt[m] = '%') then
				begin
				Inc(m);

				case fmt[m] of
					'd':
						Result:= vtInteger;
					'f':
						Result:= vtExtended;
					's':
						Result:= vtString;
					end;

				Inc(m);

				Break;
				end;

			if (ScanStr(fmt[m]) = False) then
				Break;

			Inc(m);
			end;
		end;

	begin
	N:= 1;
	m:= 1;

	Result:= 0;

	for i:= 0 to High(Pointers) do
		begin
		j:= GetFmt;

		case j of
			vtInteger:
				begin
				if GetInt > 0 then
					begin
					l:= StrToInt(string(s1));
					Move(l, Pointers[i]^, SizeOf(LongInt));
					Inc(Result);
					end
				else
					Break;
				end;

			vtExtended:
				begin
				if GetFloat > 0 then
					begin
					X:= StrToFloat(string(s1));
					Move(X, Pointers[i]^, SizeOf(Extended));
					Inc(Result);
					end
				else
					Break;
				end;

			vtString:
				begin
				if GetString > 0 then
					begin
//					Move(s1[1], PAnsiString(Pointers[i])^[1], Length(s1) + 1);
					PAnsiString(Pointers[i])^:= s1;
					UniqueString(PAnsiString(Pointers[i])^);
					Inc(Result);
					end
				else
					Break;
				end;
			else
				Break;
			end;
		end;
	end;
{$ENDIF}

function index(AString, ASubstr: AnsiString): Integer; inline;
	begin
	Result:= Pos(ASubstr, AString);
	end;


{ Sleep for given time
  NOTE: Int_time is in seconds }
//[psect(misc2$code)]
procedure sleep(int_time: unsigned);
//	var
//	bin_time: quad_type;

	begin
//	convert_time(int_time * 100, bin_time);
//	set_time(bintime:= bin_time);
//	hibernate;
	SysUtils.Sleep(int_time * 1000);
	end;

procedure PutColour(AClr: AnsiString; ARow, ACol: Integer); overload;
	begin
	if Length(AClr) > 0 then
		screen.PutAttribs(AClr, ACol, ARow);
	end;

procedure PutColour(AClr: AnsiChar; ALen: Unsigned; ARow, ACol: Integer); overload;
	var
	cs: AnsiString;

	begin
	if ALen > 0 then
		begin
		cs:= StringOfChar(AClr, ALen);
		PutColour(cs, ARow, ACol);
		end;
	end;

procedure PutColourCont(AClr: AnsiString; ALen: Unsigned; ARow, ACol: Integer);
	var
	ln: Unsigned;

	begin
	ln:= Length(AClr);
	PutColour(AClr, ARow, ACol);
	if ALen > ln then
		PutColour(AClr[ln], ALen - ln, ARow, ACol + Integer(ln));
	end;


{ Dump IO to buffer
  NOTE: Source is PUTQIO.MAR }
procedure put_buffer(StrBuff: AnsiString; AColr: AnsiString; Row, Col : integer);
	begin
	screen.BeginUpdate;
	try
		screen.GotoXY(col, row);
		screen.Write(strbuff);

//todo This isn't working where StrBuff is '' and AColr contains data
		PutColourCont(AColr, Max(Length(StrBuff), Length(AColr)), Row, Col);

		finally
		screen.EndUpdate;
		end;
	end;

{ Dump the IO buffer to terminal
  NOTE: Source is PUTQIO.MAR }
procedure put_qio;
	begin
//	dengland  Hmmm...  Don't do this for now, with all the GotoXYs and ClearEOLs.
//	Flush(output);
	end;

{ Gets single character from keyboard and returns }
//[psect(io$code)]
procedure inkey(var getchar : AnsiChar);
//	var
//	status: integer;

	begin
	put_qio;  { Dump IO buffer
Allow device driver to catch up
NOTE: Remove or comment out for VMS 4.0 or greater
	  set_time(bintime:=IO$BIN_PAUSE);
	  hibernate;
Now read }
//	qiow_read(chan:=channel,
//				func:=IO$MOR_INPUT,
//				get_char:=getchar,
//				buff_len:=1     );
	getchar:= AnsiChar(screen.ReadKey);
	msg_flag:= False;
	end;

{ Gets single character from keyboard and returns }
//[psect(io$code)]
procedure inkey_delay(var getchar: AnsiChar; delay: integer);
//	var
//	status: integer;

	begin
	put_qio;  { Dump the IO buffer
Allow device driver to catch up
NOTE: Remove or comment out for VMS 4.0 or greater
	  set_time(bintime:=IO$BIN_PAUSE);
	  hibernate;
Now read }
	getchar := null;          { Blank out return character    }
//	qiow_read(chan:=channel,
//				func:=IO$MOR_DELAY,
//				get_char:=getchar,
//				buff_len:=1,
//				delay_time:=delay );

//	dengland  I'm not sure how delay (integer) gets translated to a time so
//	don't delay/sleep.  Really, I'm going by the procedure description for this.
//	sleep();
	getchar:= AnsiChar(screen.ReadKey);
	end;


{ Flush the buffer }
//[psect(io$code)]
procedure flush;
	begin
{ Allow device driver to catch up
  NOTE: Remove or comment out for VMS 4.0 or greater
	  set_time(bintime:=IO$BIN_PAUSE);
	  hibernate;
Now flush }

//dengland
//todo  I'm not sure how to implement a flush on input, which is what I'm
//		assuming this does.
//	qiow_read(chan:=channel,func:=IO$MOR_IPURGE);
	end;


{ Flush buffer before input }
//[psect(io$code)]
procedure inkey_flush(var x: AnsiChar);
	begin
	put_qio;  { Dump the IO buffer     }

//	if (not(wizard)) then
		flush;

	inkey(x);
	end;


{ Clears given line of text }
//[psect(io$code)]
procedure erase_line(row, col : Integer);
	begin
	screen.GotoXY(col, row);
	screen.ClrEOL;
	end;

{ Clears screen at given row, column }
//[psect(io$code)]
procedure clear(row, col: integer);
	var
//	i1: integer;
	i : byte;

	begin
//	GotoXY(col, row);
//	ClrEOL;
//	for i1:= 2 to 23 do
//		begin
//		used_line[i1]:= False;
//		GotoXY(1, i1);
//		ClrEOL;
//		end;
//	put_buffer(cursor_erp, row, col);
//	put_qio; { Dump the Clear Sequence }
//	GotoXY(row, col);

	screen.BeginUpdate;
	try
		screen.GotoXY(col, row);
		screen.ClrEol;
		for i := row + 1 to 24 {screen.ScreenHeight} do
			begin
			used_line[i] := false;
			screen.GotoXY(1, i);
			screen.ClrEol
			end;

		screen.GotoXY(col, row)

		finally
		screen.EndUpdate;
		end;
	end;

{ Outputs a line to a given interpolated y,x position }
//[psect(io$code)]
procedure print(str_buff: AnsiString; str_colr: AnsiString; row: Integer;
		col: Integer);
	begin
	row:= row - panel_row_prt; { Real co-ords convert to screen positions }
	col:= col - panel_col_prt;
	used_line[row]:= True;

//	put_buffer(str_buff, row, col)
	screen.BeginUpdate;
	try
		screen.GotoXY(col, row);
		if Length(str_buff) > 0 then
			begin
			screen.Write(str_buff);
			PutColour(str_colr, row, col);
			end;
		finally
		screen.EndUpdate;
		end;
	end;

{ Outputs a line to a given y,x position }
//[psect(io$code)]
procedure prt(str_buff: AnsiString; row: Integer; col: Integer); overload;
	begin
//	put_buffer(cursor_erl+str_buff,row,col);
	screen.BeginUpdate;
	try
		screen.GotoXY(col, row);
		screen.ClrEOL;
		screen.Write(str_buff);

		finally
		screen.EndUpdate;
		end;
	end;

procedure prt(str_buff: AnsiString; AClrStr: AnsiString; row: Integer; col: Integer); overload;
	begin
	prt(str_buff, row, col);
	PutColourCont(AClrStr, Length(str_buff), row, col);
	end;


{ Outputs message to top line of screen }
//[psect(io$code)]
procedure msg_print(str_buff: AnsiString);
	var
	old_len : shortint;
	in_char : AnsiChar;

	begin
	if  msg_flag then
//	and more_flag then
		begin
		old_len := length(old_msg) + 1;
		screen.BeginUpdate;
		try
			screen.GotoXY(old_len, 1);
			screen.Write(' -more-');
			PutColour(COLR_NMHHLT, 7, 1, old_len);
			finally
			screen.EndUpdate;
			end;
		repeat
			inkey(in_char)
			until in_char in [#10, #13, #27, ' ']
		end;

	screen.BeginUpdate;
	try
		screen.GotoXY(1, 1);
		screen.ClrEol;
		screen.Write(str_buff);
		PutColour(COLR_NMHHLT, Length(str_buff), 1, 1);

		finally
		screen.EndUpdate;
		end;

	old_msg := str_buff;
	msg_flag := true
	end;

{ Prompts (optional) and returns ord value of input char
  Function returns false if <ESCAPE>,CNTL/(Y,C,Z) is input }
//[psect(io$code)]
function get_com(prompt: AnsiString; var command: AnsiChar): Boolean;
	begin
	if (length(prompt) > 1) then
		prt(prompt, STR_CLR_NMHHLT, 1, 1);

	inkey(command);

//	com_val:= ord(command);
	case command of
		#3, #25, #26, #27:
			get_com:= False;
		else
			get_com := True;
		end;

	erase_line(msg_line, msg_line);
	msg_flag := False;
	end;


{ Gets a string terminated by <RETURN>
  Function returns false if <ESCAPE>,CNTL/(Y,C,Z) is input }
//[psect(io$code)]
function get_string(var in_str: AnsiString;
		row, column, slen: Integer): Boolean; overload;
	var
	start_col,
	end_col: Integer;
//	i1    : Integer;
	x     : AnsiChar;
//	tmp   : vtype;
	flag,
	abort  : Boolean;

	begin
	abort := False;
	flag  := False;
	in_str:= '';

//dengland
//todo Check that this is correct.  I'm not sure what "Pad" is doing.  This is
//		as per BOSS.
//	put_buffer(pad(in_str,' ',slen),row,column);
	in_str:= AddCharR(' ', '', slen);
	screen.GotoXY(column, row);
	screen.Write(in_str);
	in_str:= '';

//	put_buffer('',row,column);
	screen.GotoXY(column, row);

	start_col:= column;
	end_col:= column + slen - 1;
	repeat
		inkey(x);
		case x of
			#3,#25, #26, #27 :
				Abort:= true;
			#10, #13:
				Flag:= true;
//dengland
//todo check that this is correct.  I'm not sure what #127 is.  This is as per BOSS
//			#127:
			#8:
				begin
				if (column > start_col) then
					begin
					column := column - 1;
//					put_buffer(' '+chr(8),row,column);
					screen.GotoXY(column, row);
					screen.Write(' ');
					screen.GotoXY(column, row);
//					in_str:= SubStr(in_str,1, Length(in_str) - 1);
					SetLength(in_str, Length(in_str) - 1);
					end;
				end;
			else
				begin
//				tmp := x;
//				put_buffer(tmp,row,column);
				screen.Write(x);
				in_str:= in_str + x;
				column:= column + 1;
				if (column > end_col) then
					flag:= True;
				end;
			end;
		until (flag or abort);

	if (abort) then
		get_string:= False
	else
		begin                   { Remove trailing blanks }
//		i1 := length(in_str);
//		if (i1 > 1) then
//			begin
//			while ((in_str[i1] = ' ') and (i1 > 1)) do
//				i1 := i1 - 1;
//			in_str:= substr(in_str,1,i1);
//			end;
		in_str:= AnsiString(TrimRight(string(in_str)));
		get_string:= True;
		end;
	end;

function get_string(var in_str: VType;
		row, column, slen: Integer): Boolean; overload; inline;
	var
	s: AnsiString;

	begin
	s:= in_str;
	Result:= get_string(s, row, column, slen);
	in_str:= s;
	end;

{$IFDEF DCC}
function get_string(var in_str: ShortString;
		row, column, slen: Integer): Boolean; overload;
	var
	s: AnsiString;

	begin
	s:= in_str;
	Result:= get_string(s, row, column, slen);
	in_str:= s;
	end;
{$ENDIF}

{ Return integer value of hex string }
//[psect(wizard$code)]
function get_hex_value(row, col, slen: Integer): Integer;
//	type
//	pack_type  = packed array [1..9] of AnsiChar;

	var
//	bin_val: integer;
	tmp_str: AnsiString;
//	pack_str: pack_type;

	begin
//	bin_val := 0;
//	get_hex_value := 0;
	if (get_string(tmp_str, row, col, slen)) then
//		if (length(tmp_str) <= 8) then
//			begin
//			pack_str := pad(tmp_str,' ',9);
//			if (odd(convert_hex_to_bin(pack_str,bin_val))) then
//				get_hex_value := bin_val;
//			end;
		get_hex_value := Hex2Dec(tmp_str)
	else
		get_hex_value := 0

	end;



{ Pauses for user response before returning }
//[psect(misc2$code)]
procedure pause(prt_line: Integer);
	var
	dummy: AnsiChar;

	begin
	prt('[Press any key to continue]', STR_CLR_NMHHLT, prt_line, 24);

	inkey(dummy);
//dengland
//todo Check that this is correct (copied from BOSS)
//	erase_line(24,1);
	erase_line(prt_line, 1)
	end;


{ Pauses for user response before returning
  NOTE: Delay is for players trying to roll up "perfect"
		characters.  Make them wait a bit.}
//[psect(misc2$code)]
procedure pause_exit(prt_line: Integer; delay: Integer);
	var
	dummy : AnsiChar;

	begin
	prt('[Press any key to continue, or <Control>-Z to exit]', STR_CLR_NMHHLT, prt_line, 11);

	inkey(dummy);
	erase_line(prt_line, 1);

	case dummy of
		#3, #25, #26 :
			begin
//			erase_line(prt_line,1);
			if (delay > 0) then
				sleep(delay);
//			exit;
//			Halt(0);
			raise EMoriaTerminate.Create('Pause exit');
			end;
		else
			;
		end;
//	erase_line(prt_line,1);
	end;


procedure MoriaLog(AReason: TMoriaLogReason; AText: AnsiString);
	const
	ARR_LIT_REASONS: array[TMoriaLogReason] of AnsiString = (
			'Information', 'Warning    ', 'ERROR      ');

	var
	s: AnsiString;

	begin
	if LogErrors then
		begin
		s:= AnsiString(FormatDateTime('ddmmmyyyy hh:mm:ss.zzz', Now));
		s:= s + ' ' + ARR_LIT_REASONS[AReason] + ' | ' + Trim(AText);
		Writeln(LogFile, s);
		System.Flush(LogFile);
		end;
	end;
