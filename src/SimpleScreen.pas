unit SimpleScreen;

{$IFDEF FPC}
	{$MODE DELPHI}
	{$SMARTLINK ON}
{$ENDIF}

{$H+}

interface

type
	TScreenColour = (sscBlack, sscBlue, sscGreen, sscCyan, sscRed, sscMagenta,
			sscBrown, sscLightGray, sscDarkGray, sscLightBlue, sscLightGreen,
			sscLightCyan, sscLightRed, sscLightMagenta, sscYellow, sscWhite);

const
//	Foreground colours use the lower nibble
	attrFGBlack = Ord(sscBlack);
	attrFGBlue = Ord(sscBlue);
	attrFGGreen = Ord(sscGreen);
	attrFGCyan = Ord(sscCyan);
	attrFGRed = Ord(sscRed);
	attrFGMagenta = Ord(sscMagenta);
	attrFGBrown = Ord(sscBrown);
	attrFGLightGray = Ord(sscLightGray);
	attrFGDarkGray = Ord(sscDarkGray);
	attrFGLightBlue = Ord(sscLightBlue);
	attrFGLightGreen = Ord(sscLightGreen);
	attrFGLightCyan = Ord(sscLightCyan);
	attrFGLightRed = Ord(sscLightRed);
	attrFGLightMagenta = Ord(sscLightMagenta);
	attrFGYellow = Ord(sscYellow);
	attrFGWhite = Ord(sscWhite);

//	Background colours use the upper nibble
	attrBGBlack = Ord(sscBlack) shl 4;
	attrBGBlue = Ord(sscBlue) shl 4;
	attrBGGreen = Ord(sscGreen) shl 4;
	attrBGCyan = Ord(sscCyan) shl 4;
	attrBGRed = Ord(sscRed) shl 4;
	attrBGMagenta = Ord(sscMagenta) shl 4;
	attrBGBrown = Ord(sscBrown) shl 4;
	attrBGLightGray = Ord(sscLightGray) shl 4;
//dengland  We can now define these for all supported platforms.
	attrBGDarkGray = Ord(sscDarkGray) shl 4;
	attrBGLightBlue = Ord(sscLightBlue) shl 4;
	attrBGLightGreen = Ord(sscLightGreen) shl 4;
	attrBGLightCyan = Ord(sscLightCyan) shl 4;
	attrBGLightRed = Ord(sscLightRed) shl 4;
	attrBGLightMagenta = Ord(sscLightMagenta) shl 4;
	attrBGYellow = Ord(sscYellow) shl 4;
	attrBGWhite = Ord(sscWhite) shl 4;

type
	TScreenMode = (ssmBlackWhite, ssmEightColour, ssmSixteenColour);

	TScreenPos = record
		X,
		Y: SmallInt;
	end;

	TCustomSimpleScreen = class(TObject)
	protected
		FUpdateLvl: Integer;
		FScreenMode: TScreenMode;
		FScreenWidth,
		FScreenHeight,
		FPosX,
		FPosY: SmallInt;
		FClearing: Boolean;

		procedure TestRefresh; inline;
		procedure GotoXYImmediate; virtual; abstract;

	public
		function  ReadKey: Char; virtual; abstract;
		function  KeyPressed: Boolean; virtual; abstract;
		function  WhereX: SmallInt; dynamic;
		function  WhereY: SmallInt; dynamic;
		procedure GotoXY(AX, AY: SmallInt); virtual; abstract;
		procedure ClrScr; virtual; abstract;
		procedure ClrEol; virtual; abstract;
		procedure InsLine; virtual; abstract;
		procedure DelLine; virtual; abstract;
		procedure TextColor(AColour: TScreenColour); virtual; abstract;
		procedure TextBackground(AColour: TScreenColour); virtual; abstract;
		function  TextAttrib(AForeground, ABackground: TScreenColour): Byte;
		procedure Delay(AMS: Word); virtual; abstract;
		procedure Sound(AHz: Word); virtual; abstract;
		procedure NoSound; virtual; abstract;

		procedure GetTextColours(var AForeground,
				ABackground: TScreenColour); virtual; abstract;
		procedure SetTextColours(AForeground,
				ABackground: TScreenColour); virtual; abstract;
		function  GetCheckBreak: Boolean; virtual; abstract;
		procedure SetCheckBreak(AValue: Boolean); virtual; abstract;
		function  GetScreenWidth: SmallInt; virtual;
		function  GetScreenHeight: SmallInt; virtual;
		function  GetScreenMode: TScreenMode; virtual;
		procedure SetScreenMode(AValue: TScreenMode); virtual;

		procedure CursorOn; virtual; abstract;
		procedure CursorOff; virtual; abstract;
		procedure CursorSizeBig; virtual; abstract;
		procedure CursorSizeSmall; virtual; abstract;
		procedure CursorSizeNormal; virtual; abstract;

		procedure Initialise; virtual; abstract;
		procedure BeginUpdate; dynamic;
		procedure EndUpdate; dynamic;
		procedure Refresh; virtual; abstract;
		procedure Redraw; virtual; abstract;

		procedure PutAttribs(AAttribs: AnsiString;
				ACol, ARow: SmallInt); virtual; abstract;

		procedure Read(var AString: AnsiString); virtual; abstract;
		procedure ReadLn(var AString: AnsiString); overload; virtual; abstract;
		procedure ReadLn; overload; virtual; abstract;

		procedure Write(AString: AnsiString); overload; virtual; abstract;
		procedure Write(AString,
				AAttribs: AnsiString); overload; virtual; abstract;
		procedure WriteLn(AString: AnsiString); overload; virtual; abstract;
		procedure WriteLn(AString,
				AAttribs: AnsiString); overload; virtual; abstract;
		procedure WriteLn; overload; virtual; abstract;

		property  CheckBreak: Boolean read GetCheckBreak write SetCheckBreak;
		property  ScreenWidth: SmallInt read GetScreenWidth;
		property  ScreenHeight: SmallInt read GetScreenHeight;
		property  ScreenMode: TScreenMode read GetScreenMode write SetScreenMode;
	end;

	TCustomBufferedScreen = class(TCustomSimpleScreen)
	protected
		FDirtyBuffer: array of Boolean;
		FCharBuffer: AnsiString;
		FAttrBuffer: AnsiString;
		FTextAttr: Byte;

		function  PosToBufferIdx(AX, AY: SmallInt): Integer; inline;
		procedure BufferIdxToPos(APos: Integer; var AX, AY: SmallInt); inline;

		procedure FillBuffer(AX, AY, ALength: SmallInt; AChar: AnsiChar;
				AAttrib: Byte);
		procedure MoveBuffer(AFromX, AFromY, AToX, AToY: SmallInt);

		function  IsDirty(AX, AY: SmallInt): Boolean; overload; inline;
		function  IsDirty(APos: Integer): Boolean; overload; inline;
		procedure GetBufferXY(AX, AY: SmallInt; var AChar: AnsiChar;
				var AAttr: Byte); inline;

		procedure WriteNoRefresh(AString: AnsiString); inline;

	public
		destructor Destroy; override;

		procedure GotoXY(AX, AY: SmallInt); override;
		procedure ClrScr; override;
		procedure ClrEol; override;
		procedure InsLine; override;
		procedure DelLine; override;
		procedure TextColor(AColour: TScreenColour); override;
		procedure TextBackground(AColour: TScreenColour); override;

		procedure GetTextColours(var AForeground,
				ABackground: TScreenColour); override;
		procedure SetTextColours(AForeground,
				ABackground: TScreenColour); override;

		procedure Initialise; override;
		procedure Redraw; override;

		procedure PutAttribs(AAttribs: AnsiString;
				ACol, ARow: SmallInt); override;

		procedure Write(AString: AnsiString); overload; override;
		procedure Write(AString, AAttribs: AnsiString); overload; override;
		procedure WriteLn(AString: AnsiString); overload; override;
		procedure WriteLn(AString, AAttribs: AnsiString); overload; override;
		procedure WriteLn; overload; override;
	end;

implementation


{ TCustomSimpleScreen }

procedure TCustomSimpleScreen.BeginUpdate;
	begin
	Inc(FUpdateLvl);
	end;

procedure TCustomSimpleScreen.EndUpdate;
	begin
	Dec(FUpdateLvl);

	if FUpdateLvl < 0 then
		FUpdateLvl:= 0;

	TestRefresh;
	end;

function TCustomSimpleScreen.GetScreenHeight: SmallInt;
	begin
	Result:= FScreenHeight;
	end;

function TCustomSimpleScreen.GetScreenMode: TScreenMode;
	begin
	Result:= FScreenMode;
	end;

function TCustomSimpleScreen.GetScreenWidth: SmallInt;
	begin
	Result:= FScreenWidth;
	end;

procedure TCustomSimpleScreen.SetScreenMode(AValue: TScreenMode);
	begin
	FScreenMode:= AValue;
	end;

procedure TCustomSimpleScreen.TestRefresh;
	begin
	if FUpdateLvl = 0 then
		Refresh;

	FClearing:= False;
	end;


function TCustomSimpleScreen.TextAttrib(AForeground,
		ABackground: TScreenColour): Byte;
	begin
	Result:= (Ord(ABackground) shl 4) + Ord(AForeground);
	end;

function TCustomSimpleScreen.WhereX: SmallInt;
	begin
	Result:= FPosX;
	end;

function TCustomSimpleScreen.WhereY: SmallInt;
	begin
	Result:= FPosY;
	end;

{ TCustomBufferedScreen }

procedure TCustomBufferedScreen.BufferIdxToPos(APos: Integer;
		var AX, AY: SmallInt);
	begin
	AX:= (APos mod FScreenWidth);
	AY:= (APos div FScreenWidth) + 1;
	end;

procedure TCustomBufferedScreen.ClrEol;
	var
	w: SmallInt;

	begin
	w:= FScreenWidth - FPosX + 1;
	FillBuffer(FPosX, FPosY, w, #32, FTextAttr);
//	dengland  I don't think this moves the cursor
	{ TODO 1 -odengland -cImplementation : Check if ClrEol moves the cursor }
	end;

procedure TCustomBufferedScreen.ClrScr;
	begin
	GotoXY(1, 1);
	FillBuffer(1, 1, FScreenHeight * FScreenWidth, #32, FTextAttr);

	FClearing:= True;
	TestRefresh;
	end;

procedure TCustomBufferedScreen.DelLine;
	begin
	{ TODO 3 -odengland -cFunctionality : Implement DelLine }

	TestRefresh;
	end;

destructor TCustomBufferedScreen.Destroy;
	begin
	SetLength(FDirtyBuffer, 0);
	SetLength(FCharBuffer, 0);
	SetLength(FAttrBuffer, 0);

	inherited;
	end;

procedure TCustomBufferedScreen.FillBuffer(AX, AY, ALength: SmallInt;
		AChar: AnsiChar; AAttrib: Byte);
	var
	p, i: Integer;

	begin
	p:= PosToBufferIdx(AX, AY);
	for i:= 1 to ALength do
		begin
		FDirtyBuffer[p]:= True;
		FCharBuffer[p]:= AChar;
		FAttrBuffer[p]:= AnsiChar(AAttrib);
		Inc(p);
		end;

//	Does not test refresh
	end;

procedure TCustomBufferedScreen.GetBufferXY(AX, AY: SmallInt;
		var AChar: AnsiChar; var AAttr: Byte);
	var
	p: Integer;

	begin
	p:= PosToBufferIdx(AX, AY);
	AChar:= FCharBuffer[p];
	AAttr:= Byte(FAttrBuffer[p]);
	end;

procedure TCustomBufferedScreen.GetTextColours(var AForeground,
		ABackground: TScreenColour);
	begin
	AForeground:= TScreenColour(FTextAttr and $0F);
	ABackground:= TScreenColour((FTextAttr and $F0) shr 4);
	end;

procedure TCustomBufferedScreen.GotoXY(AX, AY: SmallInt);
	begin
	FPosX:= AX;
	FPosY:= AY;

	if FUpdateLvl = 0 then
		GotoXYImmediate;
	end;

procedure TCustomBufferedScreen.Initialise;
	var
	sz: Integer;

	begin
//dengland Make it + 1 so that we can address the array 1..Length
	sz:= FScreenHeight * FScreenWidth + 1;

	SetLength(FDirtyBuffer, sz);
	SetLength(FCharBuffer, sz);
	SetLength(FAttrBuffer, sz);

	FUpdateLvl:= 0;
	FTextAttr:= TextAttrib(sscLightGray, sscBlack);
	end;

procedure TCustomBufferedScreen.InsLine;
	begin
	{ TODO 3 -odengland -cFunctionality : Implement DelLine }

	TestRefresh;
	end;

function TCustomBufferedScreen.IsDirty(APos: Integer): Boolean;
	begin
	Result:= FDirtyBuffer[APos];
	end;

function TCustomBufferedScreen.IsDirty(AX, AY: SmallInt): Boolean;
	begin
	Result:= IsDirty(PosToBufferIdx(AX, AY));
	end;

procedure TCustomBufferedScreen.MoveBuffer(AFromX, AFromY, AToX,
		AToY: SmallInt);
	begin
	{ TODO 3 -odengland -cFunctionality : Implement MoveBuffer }
	end;

function TCustomBufferedScreen.PosToBufferIdx(AX, AY: SmallInt): Integer;
	begin
	Result:= (AY - 1) * FScreenWidth + AX;
	end;

procedure TCustomBufferedScreen.PutAttribs(AAttribs: AnsiString; ACol,
		ARow: SmallInt);
	var
	p, i: Integer;

	begin
	p:= PosToBufferIdx(ACol, ARow);

	for i:= 1 to Length(AAttribs) do
		begin
//		This may be a little inefficient but I can't help it
		FDirtyBuffer[p]:= True;
		FAttrBuffer[p]:= AAttribs[i];
		Inc(p);
		end;

	TestRefresh;
	end;

procedure TCustomBufferedScreen.Redraw;
	var
	i: Integer;

	begin
	for i:= Low(FDirtyBuffer) to High(FDirtyBuffer) do
		FDirtyBuffer[i]:= True;

	Refresh;
	end;

procedure TCustomBufferedScreen.SetTextColours(AForeground,
		ABackground: TScreenColour);
	begin
	FTextAttr:= TextAttrib(AForeground, ABackground);
	end;

procedure TCustomBufferedScreen.TextBackground(AColour: TScreenColour);
	begin
	FTextAttr:= (FTextAttr and $0F) or (Ord(AColour) shl 4);
	end;

procedure TCustomBufferedScreen.TextColor(AColour: TScreenColour);
	begin
	FTextAttr:= (FTextAttr and $F0) or Byte(Ord(AColour));
	end;

procedure TCustomBufferedScreen.Write(AString, AAttribs: AnsiString);
	var
	p,
	i: Integer;

	begin
//	I need to write the string into the buffer but also write the attributes
//	up to the length I get plus the default attributes for the rest...

	{ TODO 2 -odengland -cImplementation : Implement line scrolling on write }

//	Keep a hold of the starting point
	p:= PosToBufferIdx(FPosX, FPosY);
//	Write the string data
	WriteNoRefresh(AString);

	for i:= 1 to Length(AAttribs) do
		begin
//		This may be a little inefficient but I can't help it
		FDirtyBuffer[p]:= True;
		FAttrBuffer[p]:= AAttribs[i];
		Inc(p);
		end;

//	This will be execute only if Length(AString) > Length(AAttribs)
	for i:= 1 to Length(AString) - Length(AAttribs) do
		begin
		FDirtyBuffer[p]:= True;
		FAttrBuffer[p]:= AnsiChar(FTextAttr);
		Inc(p);
		end;

	BufferIdxToPos(p, FPosX, FPosY);
	GotoXY(FPosX, FPosY);

	TestRefresh;
	end;

procedure TCustomBufferedScreen.Write(AString: AnsiString);
	var
	p: Integer;

	begin
	p:= PosToBufferIdx(FPosX, FPosY);

	WriteNoRefresh(AString);

	Inc(p, Length(AString));
	BufferIdxToPos(p, FPosX, FPosY);
	GotoXY(FPosX, FPosY);

	TestRefresh;
	end;

procedure TCustomBufferedScreen.WriteLn(AString: AnsiString);
	var
	p: Integer;

	begin
	p:= PosToBufferIdx(FPosX, FPosY);

	WriteNoRefresh(AString);

	Inc(p, Length(AString));
	BufferIdxToPos(p, FPosX, FPosY);

//	Inc(FPosY);
	GotoXY(1, FPosY + 1);

	TestRefresh;
	end;

procedure TCustomBufferedScreen.WriteLn(AString, AAttribs: AnsiString);
	var
	p,
	i: Integer;

	begin
//	I need to write the string into the buffer but also write the attributes
//	up to the length I get plus the default attributes for the rest...

	{ TODO 2 -odengland -cImplementation : Implement line scrolling on write }

//	Keep a hold of the starting point
	p:= PosToBufferIdx(FPosX, FPosY);
//	Write the string data
	WriteNoRefresh(AString);

	for i:= 1 to Length(AAttribs) do
		begin
//		This may be a little inefficient but I can't help it
		FDirtyBuffer[p]:= True;
		FAttrBuffer[p]:= AAttribs[i];
		Inc(p);
		end;

//	This will be execute only if Length(AString) > Length(AAttribs)
	for i:= 1 to Length(AString) - Length(AAttribs) do
		begin
		FDirtyBuffer[p]:= True;
		FAttrBuffer[p]:= AnsiChar(FTextAttr);
		Inc(p);
		end;

	BufferIdxToPos(p, FPosX, FPosY);
//	Inc(FPosY);
	GotoXY(1, FPosY + 1);

	TestRefresh;
	end;

procedure TCustomBufferedScreen.WriteLn;
	begin
	GotoXY(1, FPosY + 1);

	TestRefresh;
	end;

procedure TCustomBufferedScreen.WriteNoRefresh(AString: AnsiString);
	var
	p,
	i: Integer;

	begin
	{ TODO 2 -odengland -cImplementation : Needs to return if a scroll occurred }
	p:= PosToBufferIdx(FPosX, FPosY);
	for i:= 1 to Length(AString) do
		begin
		FDirtyBuffer[p]:= True;
		FCharBuffer[p]:= AString[i];
		FAttrBuffer[p]:= AnsiChar(FTextAttr);
		Inc(p);
		end;

//	Does not test refresh
	end;

end.
