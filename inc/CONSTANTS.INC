const
{Note to the Wizard:
   Tweaking these constants can *GREATLY* change the game.
   Two years of constant tuning have generated these values.
   Minor adjustments are encouraged, but you must be very
   careful not to unbalance the game.  Moria was meant to be
   challenging, not a give away.  Many adjustments can cause
   cause the game to act strangely, or even cause errors.

Current version number of Moria  }
	CUR_VERSION_MAJ   = 05;
	CUR_VERSION_MIN   = 02;
	CUR_VERSION_REV   = 00;
	CUR_VERSION_BLD   = 0099;
	CUR_VERSION_DAT   = '06JAN2014';


{  ENCRYPTION CONSTANTS
  NOTE: These numbers were pulled from the air, and can be
		changed.  If changed, characters produced from other
		versions will fail to restore.}
	ENCRYPT_SEED1 = 342342;
	ENCRYPT_SEED2 = 342944081;

{ TIME VALUES }

//dengland I want to make it 5 seconds per turn.
//	TIM_TURNS_PER_24HRS = (24 * 60 * 60) div 5;
//	TIM_TURNS_PER_12HRS = TIM_TURNS_PER_24HRS div 2;
//dengland Currently (according to the town behaviour) there are 5000 turns in
//		12 hours.  This is 8.64 seconds per turn.
	TIM_TURNS_PER_12HRS = 5000;


{ DUNGEON SIZE PARAMETERS }
	MAX_HEIGHT    = 66;     { Multiple of 11; >= 22 }
	MAX_WIDTH     = 198;    { Multiple of 33; >= 66 }
	SCREEN_HEIGHT = 22;
	SCREEN_WIDTH  = 66;

	SIGHT_RADIUS = 20;
	EFF_SIGHT_RADIUS = 5;
	X_MARGIN = 9;
	Y_MARGIN = 5;


{ OUTPUT DUNGEON SECTION SIZES }
	OUTPAGE_HEIGHT= 44;{ 44 lines of dungeon/section }
	OUTPAGE_WIDTH = 99;{ 100 columns of dungeon/section }

{ DUNGEON GENERATION VALUES
  NOTE: The entire design of dungeon can be changed by only
		slight adjustments here. }
	DUN_TUN_RND  = 36; { Rand dir (4 is min) }
	DUN_TUN_CHG  = 70; { Chance of changing dir (99 max) }
	DUN_TUN_FND  = 12; { Distance for auto find to kick in }
	DUN_TUN_CON  = 15; { Chance of extra tunneling }
	DUN_ROO_MEA  = 32; { Mean of # of rooms, std dev=2 }
	DUN_TUN_PEN  = 25; { % chance of room doors }
	DUN_TUN_JCT  = 15; { % chance of doors at tunnel junctons }
	DUN_STR_DEN  = 5;  { Density of streamers }
	DUN_STR_RNG  = 2;  { Width of streamers }
	DUN_STR_MAG  = 3;  { # of magma streamers }
	DUN_STR_MC   = 95; { 1/x chance of treasure/magma }
	DUN_STR_QUA  = 2;  { # of quartz streamers }
	DUN_STR_QC   = 55; { 1/x chance of treasure/quartz }
	DUN_UNUSUAL  = 300;{ Level/x chance of unusual room }

{ STORE CONSTANTS }
	MAX_OWNERS   = 21;   { # of owners to choose from }

//fixme This should be 8 to include the bank.
	MAX_STORES   = 7;    { # of different stores }

	STORE_INVEN_MAX = 24;{ Max # of discrete objs in inven  }
	STORE_CHOICES = 26;  { # of items to choose stock from  }
	STORE_MAX_INVEN = 20;{ Max diff objs in stock before auto sell}
	STORE_MIN_INVEN = 14;{ Min diff objs in stock before auto buy}
	STORE_TURN_AROUND = 3;{ Amt of buying and selling normally }
	BMSTORE_TURN_AROUND = 20;{ Amt of buying and selling normally }
	INVEN_INIT_MAX = 106;{ Size of store init array }
	BLK_MKT_MAX = 176;{ Size of black mkt store array }
	COST_ADJ = 1.00; { Adjust prices for buying and selling  }

{ TREASURE CONSTANTS }
//	INVEN_MAX = 35;     { Size of inventory array(DO NOT CHANGE)}
	MAX_OBJ_LEVEL = 50; { Max level of magic in dungeon }
	OBJ_GREAT = 14;     { 1/n Chance of item being a Great Item }
	MAX_OBJECTS = 351;  { # of objects for universe }
	MAX_GOLD  = 18;     { # of different types of gold }
	MAX_TALLOC = 225;   { Max objects/level }
	TREAS_ROOM_ALLOC = 8;{ Amt of objects for rooms }
	TREAS_ANY_ALLOC  = 2;{ Amt of objects for corridors }

//dengland Making this 8 instead of 2
	TREAS_GOLD_ALLOC = 8;{ Amt of gold (and gems) }

{ MAGIC TREASURE GENERATION CONSTANTS
  NOTE: Number of special objects, and degree of enchantments
		can be adjusted here. }
	OBJ_STD_ADJ = 1.25; { Adjust STD/level }
	OBJ_STD_MIN = 7;    { Minimum STD }
	OBJ_TOWN_LEVEL = 5; { Town object generation level }
	OBJ_BASE_MAGIC = 20;{ Base amt of magic }
	OBJ_BASE_MAX = 70;  { Max amt of magic }
	OBJ_DIV_SPECIAL = 7;{ magic_chance/# = special magic }
	OBJ_DIV_CURSED = 1.3;{ magic_chance/# = cursed items }

{ CONSTANTS DESCRIBING LIMITS OF CERTAIN OBJECTS }
	OBJ_LAMP_MAX = 15000; { Max amt that lamp can be filled}
	OBJ_BOLT_RANGE = 18;  { Max range of bolts and balls }

{ CREATURE CONTANTS }
//		max_creatures = 278; { # of creatures defined for univ }
	MAX_MALLOC = 100 + 1;{ Max that can be allocated }
	MAX_MALLOC_CHANCE = 170; { 1/x chance of new monster each round }
	MAX_MONS_LEVEL = 40; { Max level of creatures }
	MAX_SIGHT =  20; { Max dist a creature can be seen }
	MAX_SPELL_DIS = 20; { Max dist creat. spell can be cast }
	MAX_MON_MULT = 80; { Max reproductions on a level }
	MON_MULT_ADJ = 7; { High value slows multiplication }
	MON_NASTY = 50; { Dun_level/x chance of high level creature }
	MIN_MALLOC_LEVEL = 20; { Min # of monsters/level }
	MIN_MALLOC_TD = 4; { # of people on town level (day) }
	MIN_MALLOC_TN = 8; { # of people on town level (night)}
	WIN_MON_TOT = 1; { Total number of "win" creatures }
	WIN_MON_APPEAR = 70; { Level where winning creatures begin }
	MON_SUMMON_ADJ = 2; { Adjust level of summoned creatures }
	MON_DRAIN_LIFE = 2; { % of player exp drained/hit }

{ TRAP CONSTANTS }
	MAX_TRAPA = 19; { # of defined traps }
	MAX_TRAPB = 20; { Includes secret doors }

{ DESCRIPTIVE CONSTANTS }
	MAX_COLORS = 67; { Used with potions }
	MAX_MUSH = 29;   { Used with mushrooms }
	MAX_WOODS = 41;  { Used with staves }
	MAX_METALS = 31; { Used with wands }
	MAX_ROCKS = 52;  { Used with rings }
	MAX_AMULETS = 39;{ Used with amulets }
	MAX_SYLLABLES = 153; { Used with scrolls }

{ PLAYER CONSTANTS }
	MAX_PLAYER_LEVEL = 46;  { Max possible char level }
	MAX_RACES = 8; { # of defined races }
	MAX_CLASS = 8; { # of defined classes }
	USE_DEVICE = 3;{ x> Harder devices x< Easier devices }
	MAX_BACKGROUND = 128; { # of types of histories for univ }

	PLAYER_FOOD_FULL = 7500;{ Getting full }
	PLAYER_FOOD_MAX = 12500;{ Max food value, beyond is wasted  }
	PLAYER_FOOD_FAINT = 180; { Char begins fainting }
	PLAYER_FOOD_WEAK = 650; { Warn player that he is getting very low}
	PLAYER_FOOD_ALERT = 1100; { Warn player that he is getting low }

	PLAYER_REGEN_FAINT = 0.002; { Regen factor  fainting }
	PLAYER_REGEN_WEAK = 0.004; { Regen factor  weak }
	PLAYER_REGEN_NORMAL = 0.006; { Regen factor  full }
	PLAYER_REGEN_HPBASE = 0.025; { Min amt hp regen }
	PLAYER_REGEN_MNBASE = 0.075; { Min amt mana regen }
	PLAYER_WEIGHT_CAP = 130; { 100*(1/10 pounds)/strength pt }
	PLAYER_EXIT_PAUSE = 5; { Pause time before player can re-roll  }

{ BASE TO HIT CONSTANTS }
	BTH_LEV_ADJ = 3; { Adjust BTH/level }
	BTH_PLUS_ADJ = 3; { Adjust BTH/plus-to-hit }
	BTH_HIT = 12; { Automatic hit; 1/bth_hit }

{ MISC }
	NULL = AnsiChar(0);

	ARR_LIT_BOOL: array[Boolean] of AnsiString = ('False', 'True ');
	STR_TOK_IMGPTH = './dat/';


//	These definitions use the colours above to define the colours for particular
//	elements.
	COLR_NORMAL = AnsiChar(attrBGBlack + attrFGLightGray);
	COLR_NMHHLT = AnsiChar(attrBGBlack + attrFGWhite);
	COLR_NMDLMT = AnsiChar(attrBGBlack + attrFGLightCyan);

	COLR_BADSTA = AnsiChar(attrBGBlack + attrFGRed);
	COLR_PLSSTA = AnsiChar(attrBGBlack + attrFGCyan);

	COLR_GOODPC = AnsiChar(attrBGBlack + attrFGGreen);
	COLR_FAIRPC = AnsiChar(attrBGBlack + attrFGBrown);
	COLR_POORPC = AnsiChar(attrBGBlack + attrFGRed);

//	These define the basic wall colours
	COLR_WALL_BKGRND = attrBGDarkGray;
	COLR_WALL_NORMAL = AnsiChar(COLR_WALL_BKGRND + attrFGLightGray);
	COLR_WALL_QUARTZ = AnsiChar(COLR_WALL_BKGRND + attrFGBrown);
	COLR_WALL_DRKMGM = AnsiChar(COLR_WALL_BKGRND + attrFGRed);

//	These are used for objects
	COLR_TRES_RNWARD = AnsiChar(attrBGLightGray + attrFGRed);
	COLR_TRES_VISTRP = AnsiChar(attrBGBlack + attrFGRed);
	COLR_TRES_STAIRS = AnsiChar(attrBGBlack + attrFGCyan);
	COLR_TRES_RUBBLE = AnsiChar(attrBGBrown + attrFGLightGray);
	COLR_TRES_DORBKG = attrBGBrown;
	COLR_TRES_OPNDOR = AnsiChar(attrBGBlack + attrFGBrown);
	COLR_TRES_CLSDOR = AnsiChar(COLR_TRES_DORBKG + attrFGLightGray);

//	These are used for the stores
	COLR_STOR_TBLBG1 = attrBGDarkGray;
//	We need to define this one differently for Linux since the colours there
//	are slightly different generally and the cyan looks too bright.
{$IFNDEF LINUX}
	COLR_STOR_TBLBG2 = attrBGCyan;
{$ELSE}
	COLR_STOR_TBLBG2 = attrBGBlue;
{$ENDIF}
	COLR_STOR_TBLFG1 = attrFGWhite;
	COLR_STOR_TITLE1 = AnsiChar(attrBGWhite + attrFGBlack);
	COLR_STOR_ITMHDR = AnsiChar(attrBGLightGray + attrFGBlack);

//	Player colours (backgrounds)
	COLR_PLYR_NORMAL = AnsiChar(attrBGGreen);
	COLR_PLYR_NORESP = AnsiChar(attrBGBrown);

//	Creature colours (backgrounds)
	COLR_CREA_INNCNT = AnsiChar(attrBGCyan);
	COLR_CREA_HOSTLE = AnsiChar(attrBGRed);
	COLR_CREA_NORESP = AnsiChar(attrBGBrown);

//	Default colour for area effects
	COLR_BALL_NORMAL = AnsiChar(attrBGMagenta + attrFGWhite);

//	String short-cuts
	STR_CLR_NORMAL = AnsiString(COLR_NORMAL);
	STR_CLR_NMHHLT = AnsiString(COLR_NMHHLT);
	STR_CLR_NMDLMT = AnsiString(COLR_NMDLMT);


