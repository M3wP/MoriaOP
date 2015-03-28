{ Moria Version 5.02    COPYRIGHT (c) Robert Alan Koeneke
						Public Domain
				   Modified EXTENSIVELY by:
					Matthew W. Koch   -MWK
					Kendall R. Sears  -Opusii
					Daniel England - DENGLAND
					Michał Bieliński - MB
				   with minor help from:
					Russell E. Billings -REB
					David G. Strubel    -DGS

		I lovingly dedicate this game to hackers and adventurers
		everywhere...

		Designer and Programmer : Robert Alan Koeneke
								  University of Oklahoma
		Assitant Programmer     : Jimmey Wayne Todd
								  University of Oklahoma

		Delphi/Free Pascal port : Daniel England

		Moria may be copied and modified freely as long as the above
		credits are retained.  No one who-so-ever may sell or market
		this software in any form without the expressed written consent
		of the author Robert Alan Koeneke.}

program Moria;

//done? Fix the display of monster/player status after the stats changes in dungeon.
//done? Display shop pages with colours
//done? Make sure all termination messages have a new line after them
//done? Better starting items for the classes.
//done? Change the free action status for equipping and switching (not free for
//		equipping and free for swapping)
//done? New inventory system
//done? Make the death and win graphics in colour

//in_progress Implement "services" from the various stores and be sure certain
//		things are always available.  Implement item identification as part of
//		selling items in the stores.

//todo Be sure that magical items always have some magical properties (not all
//		0)
//todo Change auto pickup to only pickup money (picking up all that rubbish you
//		walk over is a pest).  Need to add "g-et" command.
//todo Change the colours of items on ground (and inv?) to be based upon
//		status/type
//todo Use new list view for spell lists

//todo Scale difficulty of picking/bashing doors so is easier on lower levels?
//todo Change the way potion level modifiers work

//todo Make all message lines coloured white.

//todo Implement BOSS-alike character generation with base stats and points that
//		can be distributed.
//todo Put wizard flag into player data so once used, player is always marked as
//		having "cheated".
//todo Implement three difficulty settings (Easy, Normal, Hard) in character
//		generation.
//todo Implement spell learned separate from spell data like object identify so
//		that spell tables can be made const.
//todo Read objects from data files.
//todo Implement effect objects and animation, too?
//todo Implement morale.
//todo Implement AD&D cleave and whirlwind feat equivalents (just increase number
//		of attacks with level increase and allow them to spread over close
//		monsters?)
//todo Implement AD&D-alike attacks of opportunity? (need morale?)
//todo Implement henchmen/companions for an AD&D-alike Druid class?
//		(familiars for mages?)
//todo Implement Barbarian or Necromancer class?  Need to do some research about
//		what is the best class to implement and how to do it.


{$IFDEF MSWINDOWS}
	{$DEFINE WINDOWS}
{$ENDIF}

{$IFDEF WINDOWS}
	{$APPTYPE CONSOLE}
	{$SETPEFLAGS $0001}
{$ENDIF}

{$IFDEF FPC}
	{$MODE DELPHI}
	{$INCLUDEPATH inc}
{$ENDIF}

{$H+}
{$V-}
{$I-}

uses
{$IFDEF WINDOWS}
	Windows,
{$ENDIF }
{$IFDEF UNIX}
	Unix,
{$ENDIF }
	SysUtils,
	Classes,
	IniFiles,
	StrUtils,
	Math,
{$IFDEF DCC}
	System.AnsiStrings,
{$ENDIF }
{$IFDEF WINDOWS}
	Console in 'src\win\Console.pas',
{$ELSE}
	crt in 'src\unix\crt.pas',
{$ENDIF }
	SimpleScreen in 'src\SimpleScreen.pas',
	CRTScreen in 'src\CRTScreen.pas',
	ScrListView in 'src\ScrListView.pas';

//	Globals
	{$INCLUDE CONSTANTS.INC}
	{$INCLUDE TYPES.INC}
	{$INCLUDE TABLES.INC}
	{$INCLUDE VARIABLES.INC}

//	Libraries of routines
	{$INCLUDE IO.INC}
	{$INCLUDE MISC.INC}
	{$INCLUDE DEATH.INC}
	{$INCLUDE HELP.INC}
	{$INCLUDE DESC.INC}
	{$INCLUDE INVENTORY.INC}

	{$INCLUDE USERID.INC}        {-MWK}
	{$INCLUDE BLACK_MARKET.INC}  {-WLP}
	{$INCLUDE FILES.INC}
	{$INCLUDE STORE1.INC}
	{$INCLUDE SAVE.INC}
	{$INCLUDE CREATE.INC}
	{$INCLUDE GENERATE.INC}
	{$INCLUDE MORIA.INC}
	{$INCLUDE DATAFILES.INC}

//	Core routines
	{$INCLUDE MAIN.INC}

begin
try
	screen:= TCRTScreen.Create;
	try
		try
			Configure;

			Initialise;

			Intro;

			Start;

			Play;

			Finalise;

			except
			on E: EMoriaTerminate do
				MoriaLog(mlrInform, 'Exit:  ' + AnsiString(E.Message));
			on E: Exception do
				begin
				MoriaLog(mlrError, 'Runtime exception.  ' +
						AnsiString(E.ClassName) + ':  ' +
						AnsiString(E.Message));
				raise E;
				end;
			end;

		finally
		screen.Free;

		if LogErrors then
			begin
			MoriaLog(mlrInform, '--- Program terminating');
			LogErrors:= False;
			CloseFile(LogFile);
			end;
		end;

	except
	on E: Exception do
		begin
		Writeln('Error ' + E.ClassName + ':  ' + E.Message);

		CrashDump;

		Readln;
		end;
	end;
end.
