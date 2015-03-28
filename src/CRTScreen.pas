unit CRTScreen;

//FPC and DCC differ so make them the same.
{$IFDEF MSWINDOWS}
	{$DEFINE WINDOWS}
{$ENDIF}

{$IFDEF FPC}
	{$MODE DELPHI}
	{$SMARTLINK ON}
{$ENDIF}

{$H+}

interface

uses
	SimpleScreen,
{$IFDEF WINDOWS}
	Console;
{$ELSE}
	crt;
{$ENDIF}

type
	TCRTScreen = class(TCustomBufferedScreen)
	protected
		procedure GotoXYImmediate; override;

		procedure DoSetCrtTextAttr(AAttr: Byte); inline;

	public
		constructor Create;
		destructor  Destroy; override;

		function  ReadKey: Char; override;
		function  KeyPressed: Boolean; override;
		procedure TextColor(AColour: TScreenColour); override;
		procedure TextBackground(AColour: TScreenColour); override;

		procedure SetTextColours(AForeground,
				ABackground: TScreenColour); override;

		function  GetCheckBreak: Boolean; override;
		procedure SetCheckBreak(AValue: Boolean); override;
		procedure SetScreenMode(AValue: TScreenMode); override;

		procedure Delay(AMS: Word); override;
		procedure Sound(AHz: Word); override;
		procedure NoSound; override;

		procedure CursorOn; override;
		procedure CursorOff; override;
		procedure CursorSizeBig; override;
		procedure CursorSizeSmall; override;
		procedure CursorSizeNormal; override;

		procedure Refresh; override;

		procedure Read(var AString: AnsiString); override;
		procedure ReadLn(var AString: AnsiString); override;
		procedure ReadLn; override;
	end;



implementation


{ TCRTScreen }

constructor TCRTScreen.Create;
	begin
	inherited Create;

{$IFDEF WINDOWS}
	FScreenHeight:= Console.ScreenHeight;
	FScreenWidth:= Console.ScreenWidth;
{$ELSE}
	FScreenHeight:= crt.ScreenHeight;
	FScreenWidth:= crt.ScreenWidth;

	FScreenMode:= ssmSixteenColour;
	SetScreenMode(FScreenMode);
{$ENDIF}

	Initialise;
	end;

procedure TCRTScreen.CursorOff;
	begin
{$IFDEF WINDOWS}
	{ TODO 3 -odengland -cFunctionality : Implement CursorOff for Windows }
{$ELSE}
	crt.CursorOff;
{$ENDIF}
	end;

procedure TCRTScreen.CursorOn;
	begin
{$IFDEF WINDOWS}
	{ TODO 3 -odengland -cFunctionality : Implement CursorOn for Windows }
{$ELSE}
	crt.CursorOn;
{$ENDIF}
	end;

procedure TCRTScreen.CursorSizeBig;
	begin
	CursorBig;
	end;

procedure TCRTScreen.CursorSizeNormal;
	begin
	{ TODO 3 -odengland -cFunctionality : Implement CursorSizeNormal }
	end;

procedure TCRTScreen.CursorSizeSmall;
	begin
	{ TODO 3 -odengland -cFunctionality : Implement CursorSizeSmall }
	end;

procedure TCRTScreen.Delay(AMS: Word);
	begin
{$IFDEF WINDOWS}
	Console.Delay(AMS);
{$ELSE}
	crt.Delay(AMS);
{$ENDIF}
	end;

destructor TCRTScreen.Destroy;
	begin

	inherited;
	end;

procedure TCRTScreen.DoSetCrtTextAttr(AAttr: Byte);
	begin
	TextAttr:= Byte(AAttr);
{$IFDEF WINDOWS}
	Console.TextBackground((Byte(AAttr) and $F0) shr 4);
{$ELSE}
	crt.TextBackground((Byte(AAttr) and $F0) shr 4);
{$ENDIF}
	end;

function TCRTScreen.GetCheckBreak: Boolean;
	begin
{$IFDEF WINDOWS}
	Result:= Console.CheckBreak;
{$ELSE}
	Result:= crt.CheckBreak;
{$ENDIF}
	end;

procedure TCRTScreen.GotoXYImmediate;
	begin
{$IFDEF WINDOWS}
	Console.GotoXY(FPosX, FPosY);
{$ELSE}
	crt.GotoXY(FPosX, FPosY);
{$ENDIF}
	end;

function TCRTScreen.KeyPressed: Boolean;
	begin
{$IFDEF WINDOWS}
	Result:= Console.KeyPressed;
{$ELSE}
	Result:= crt.KeyPressed;
{$ENDIF}
	end;

procedure TCRTScreen.NoSound;
	begin
{$IFDEF WINDOWS}
	Console.NoSound;
{$ELSE}
	crt.NoSound;
{$ENDIF}
	end;

procedure TCRTScreen.Read(var AString: AnsiString);
	begin
	{ TODO 2 -odengland -cImplementation : Need to implement screen echo }
	System.Read(AString);
	end;

function TCRTScreen.ReadKey: Char;
	begin
{$IFDEF WINDOWS}
	Result:= Console.ReadKey;
{$ELSE}
	Result:= crt.ReadKey;
{$ENDIF}
	end;

procedure TCRTScreen.ReadLn;
	begin
	System.Readln;
	end;

procedure TCRTScreen.ReadLn(var AString: AnsiString);
	begin
	{ TODO 2 -odengland -cImplementation : Need to implement screen echo }
	System.Readln(AString);
	end;

procedure TCRTScreen.Refresh;
	var
	d, n: Boolean;
	i, j, m: Integer;
	x, y: SmallInt;
	a, b, l: Byte;

	begin
//	Don't ever fill the last char to prevent scrolling.  Need something better
//	than this!  Actually, only do it when FClearing is True.

	m:= FScreenHeight * FScreenWidth;
	if not FClearing then
		Dec(m);

	i:= 1;
	j:= 0;
	d:= IsDirty(i);
	n:= True;
	l:= 0;
	repeat
		if d then
			begin
			if j <> i then
				begin
				BufferIdxToPos(i, x, y);
{$IFDEF WINDOWS}
				Console.GotoXY(x, y);
{$ELSE}
				crt.GotoXY(x, y);
{$ENDIF}
				j:= i;
				end;

			if n then
				begin
				l:= Byte(FAttrBuffer[i]);

				if FScreenMode = ssmSixteenColour then
					DoSetCrtTextAttr(l)
				else if FScreenMode = ssmEightColour then
					begin
//					Allow bold
					a:= Byte(l) and $0F;
					b:= Byte(l) shr 4;
//					Don't allow blinking
					b:= b and $07;

					a:= a or (b shl 4);
					DoSetCrtTextAttr(a);
					end
				else
					begin
					a:= Byte(l) and $0F;
//					b:= Byte(l) shr 4;

//					Translate foreground to black and white.  Allow bold.  Don't
//					allow black.
					if a > 0 then
						begin
						if a <> 8 then
							if a > 7 then
								a:= 15
							else
								a:= 7;
						end
					else
						a:= 7;

//					Translate background to black only.  Don't allow any colour
//					or blink.
//					b:= 0;

//					if b > 0 then
//						if b <> 8 then
//							if b > 7 then
//								b:= 7
//							else
//								b:= 7
//						else
//							b:= 0;

//					a:= a {or (b shl 4)};

					DoSetCrtTextAttr(a);
					end;
				 end;

			System.Write(FCharBuffer[i]);

			FDirtyBuffer[i]:= False;
			end;

		Inc(i);
		if i <= m then
			begin
			d:= IsDirty(i);
			n:= l <> Byte(FAttrBuffer[i]);
			end;

		if d and (j = (i - 1)) then
			j:= i;

		until i > m;

{$IFDEF WINDOWS}
	Console.GotoXY(FPosX, FPosY);
{$ELSE}
	crt.GotoXY(FPosX, FPosY);
{$ENDIF}
	end;

procedure TCRTScreen.SetCheckBreak(AValue: Boolean);
	begin
{$IFDEF WINDOWS}
	Console.CheckBreak:= AValue;
{$ELSE}
	crt.CheckBreak:= AValue;
{$ENDIF}
	end;

procedure TCRTScreen.SetScreenMode(AValue: TScreenMode);
	begin
{$IFNDEF WINDOWS}
	if AValue = ssmSixteenColour then
		UseHighIntensityColours:= True
	else
		UseHighIntensityColours:= False;
{$ENDIF}

	if AValue <> FScreenMode then
		Redraw;

	inherited;
	end;

procedure TCRTScreen.SetTextColours(AForeground, ABackground: TScreenColour);
	begin
	inherited;

	DoSetCrtTextAttr(FTextAttr);
	end;

procedure TCRTScreen.Sound(AHz: Word);
	begin
{$IFDEF WINDOWS}
	Console.Sound(AHz);
{$ELSE}
	crt.Sound(AHz);
{$ENDIF}
	end;

procedure TCRTScreen.TextBackground(AColour: TScreenColour);
	begin
	inherited;

	DoSetCrtTextAttr(FTextAttr);
	end;

procedure TCRTScreen.TextColor(AColour: TScreenColour);
	begin
	inherited;

	DoSetCrtTextAttr(FTextAttr);
	end;

end.
