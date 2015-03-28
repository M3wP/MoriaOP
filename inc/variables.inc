var
	screen: TCustomSimpleScreen;
	LogFile: Text;
	LogErrors: Boolean;
	DebugSaveOnLevel: Boolean;

	max_weight : {[psect(weight$code)]} integer;
	balrog_ded : {[psect(balrog$code)]} boolean;
	max_creatures : {[psect(moria$data)]} integer; {max monsters allowed}
	hexnum : {[psect(moria$data)]} unsigned; { conversion variable }

	indx : {[psect(moria$data)]} integer; { index counters }
	Opusii: Text;

	REB_Iggy : {[psect(moria$data)]} integer;
	player_max_exp : {[psect(player$data)]} integer;  { Max exp possible    }

//	dengland  This has to be wrapped since it was used in the RandInt macro.
//	seed : {[psect(player$data),global]} unsigned; { Contains seed # }

	randes_seed : {[psect(setup$data)]} LongInt; { For encoding colors }
	town_seed : {[psect(generate$data)]} LongInt; { Seed for town genera}

	cur_height,
	cur_width : {[psect(player$data)]} integer;  { Cur dungeon size}
	dun_level : {[psect(player$data)]} integer;  { Cur dungeon level   }
	missle_ctr : {[psect(player$data)]} integer = 0;  { Counter for missles }
	msg_line : {[psect(player$data)]} integer;  { Contains message txt}
	msg_flag : {[psect(player$data)]} boolean;  { Set with first msg  }
	old_msg : {[psect(player$data)]} vtype;    { Last message    }
	generate : {[psect(setup$data)]} boolean;  { Generate next level }
	death : {[psect(player$data)]} boolean = False;  { True if died    }
	died_from : {[psect(player$data)]} vtype;    { What killed him }
	find_flag : {[psect(player$data)]} boolean;  { Used in MORIA   }

//dengland There was a clash (it was delcared in the dungeon routine as well
//		but it is needed here now).
	reset_flag : {[psect(player$data)]} Boolean;  { Used in MORIA   }
//	reset_flag:         boolean; { Do not move creatures }

	cave_flag : {[psect(player$data)]} boolean;  { Used in GET_PANEL   }
	light_flag : {[psect(player$data)]} boolean;  { Used in MOVE_LIGHT  }

//dengland This was only used in one place and only assigned.
//	redraw : {[psect(player$data)]} boolean;  { For redraw screen   }

	stat_column : {[psect(player$data)]} integer = 1;  { Column for stats    }
	print_stat : {[psect(player$data)]} unsigned = 0; { Flag for stats  }
	turn : {[psect(player$data)]} integer = 0;  { Cur turn of game    }

//dengland I'm putting this into the player's status bits.
//	wizard : {[psect(player$data)]} boolean = False;  { Wizard flag }

//	dengland values.inc initialised this to all False but this will happen for
//	us, anyway?  I've made this up to 24 to properly clear the screen.
	used_line : {[psect(player$data)]} array [2..24] of Boolean;

{  Following are calculated from max dungeon sizes  }
	max_panel_rows,
	max_panel_cols : {[psect(player$data)]} integer;
	quart_height,
	quart_width : {[psect(player$data)]} integer;
	panel_row,
	panel_col : {[psect(player$data)]} integer;
	panel_row_min,
	panel_row_max : {[psect(player$data)]} integer;
	panel_col_min,
	panel_col_max : {[psect(player$data)]} integer;
	panel_col_prt,
	panel_row_prt : {[psect(player$data)]} integer;

{  Following are all floor definitions  }
	cave : {[psect(moria$data)]} array [1..MAX_HEIGHT] of row_floor;

{ Following are player variables    }
		{ PLAYER RECORD IS SPECIAL }
	py : {[psect(player$data)]} player_type = (
		misc : (
			name: ' ';
			race: ' ';
			sex: ' ';
			title: ' ';
			tclass:' ';
			max_exp: 0;
			exp: 0;
			au: 0;
			age: 0;
			ht: 0;
			wt: 0;
			lev: 0;
			max_lev: 0;
			srh: 0;
			fos: 0;
			bth: 0;
			bthb: 0;
			mana: 0;
			mhp: 0;
			ptohit: 0;
			ptodam: 0;
			pac: 0;
			ptoac: 0;
			dis_th: 0;
			dis_td: 0;
			dis_ac: 0;
			dis_tac: 0;
			disarm: 0;
			save: 0;
			sc: 0;
			pclass: 0;
			prace: 0;
			hitdie: 0;
			stl: 0;
			expfact: 0;
			cmana: 0;
			chp: 0;
			history: ('','','','','')
			);
		stat : (
			str: 0;
			cstr: 0;
			astr: 0;
			dex: 0;
			cdex: 0;
			adex: 0;
			con: 0;
			ccon: 0;
			acon: 0;
			int: 0;
			cint: 0;
			aint: 0;
			wis: 0;
			cwis: 0;
			awis: 0;
			chr: 0;
			cchr: 0;
			achr: 0;
			avail: 0;
			);
		flags : (
			status: 0;
			rest: 0;
			blind: 0;
			paralysis: 0;
			confused: 0;
			food: PLAYER_FOOD_MAX;
			food_digested: 2;
			protection: 0;
			speed: 0;
			fast: 0;
			slow: 0;
			afraid: 0;
			poisoned: 0;
			image: 0;
			protevil: 0;
			invuln: 0;
			hero: 0;
			shero: 0;
			blessed: 0;
			resist_heat: 0;
			resist_cold: 0;
			detect_inv: 0;
			word_recall: 0;
			see_infra: 0;
			tim_infra: 0;
			see_inv: False;
			teleport: False;
			free_act: False;
			slow_digest: False;
			aggravate: False;
			fire_resist: False;
			cold_resist: False;
			acid_resist: False;
			regenerate: False;
			lght_resist: False;
			ffall: False;
			sustain_str: False;
			sustain_int: False;
			sustain_wis: False;
			sustain_con: False;
			sustain_dex: False;
			sustain_chr: False;
			confuse_monster: False
		));

	acc_exp : {[psect(player$data)]} real = 0.0; { Accumulator for fractional exp}
	char_row : {[psect(player$data)]} integer = 0;
	char_col : {[psect(player$data)]} integer = 0;
	com_val : {[psect(player$data)]} integer;

	total_winner : {[psect(setup$data)]} boolean = False;

	store : {[psect(store$data)]} array [1..MAX_STORES] of store_type;
	storeLst: TScrListView;

	mage_spell : {[psect(player$data)]} array [1..MAX_CLASS] of array [1..31] of spell_type = (
			{ Warrior }
		   ((sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)),

			{ Mage }
			((sname: 'Magic Missle'; slevel: 1; smana: 1; sexp: 5; sfail: 22; learned:  False),
			 (sname: 'Detect Monsters'; slevel: 1; smana: 1; sexp: 5; sfail: 27; learned:  False),
			 (sname: 'Phase Door'; slevel: 1; smana: 2; sexp: 4; sfail: 22; learned:  False),
			 (sname: 'Light Area'; slevel: 1; smana: 2; sexp: 4; sfail: 25; learned:  False),
			 (sname: 'Cure Light Wounds'; slevel: 3; smana: 3; sexp: 8; sfail: 25; learned:  False),
			 (sname: 'Find Hidden Traps/Doors'; slevel: 3; smana: 3; sexp: 6; sfail: 45; learned:  False),
			 (sname: 'Stinking Cloud'; slevel: 3; smana: 4; sexp: 8; sfail: 27; learned:  False),
			 (sname: 'Confusion'; slevel: 3; smana: 4; sexp: 6; sfail: 32; learned:  False),
			 (sname: 'Lightning Bolt'; slevel: 5; smana: 4; sexp: 25; sfail: 32; learned:  False),
			 (sname: 'Trap/Door Destruction'; slevel: 5; smana: 5; sexp: 30; sfail: 37; learned:  False),
			 (sname: 'Sleep I'; slevel: 5; smana: 5; sexp: 20; sfail: 32; learned:  False),
			 (sname: 'Cure Poison'; slevel: 5; smana: 5; sexp: 25; sfail: 37; learned:  False),
			 (sname: 'Teleport Self'; slevel: 7; smana: 6; sexp: 35; sfail: 37; learned:  False),
			 (sname: 'Remove Curse'; slevel: 7; smana: 6; sexp: 40; sfail: 50; learned:  False),
			 (sname: 'Frost Bolt'; slevel: 7; smana: 6; sexp: 50; sfail: 40; learned:  False),
			 (sname: 'Turn Stone to Mud'; slevel: 9; smana: 7; sexp: 75; sfail: 44; learned:  False),
			 (sname: 'Identify'; slevel: 9; smana: 7; sexp: 75; sfail: 95; learned:  False),
			 (sname: 'Recharge Item I'; slevel: 9; smana: 7; sexp: 90; sfail: 70; learned:  False),
			 (sname: 'Sleep II'; slevel: 9; smana: 7; sexp: 75; sfail: 45; learned:  False),
			 (sname: 'Polymorph Other'; slevel: 11; smana: 7; sexp: 100; sfail: 45; learned:  False),
			 (sname: 'Create Food'; slevel: 11; smana: 7; sexp: 75; sfail: 50; learned:  False),
			 (sname: 'Sleep III'; slevel: 13; smana: 7; sexp: 90; sfail: 50; learned:  False),
			 (sname: 'Fire Bolt'; slevel: 15; smana: 9; sexp: 100; sfail: 50; learned:  False),
			 (sname: 'Slow Monster'; slevel: 17; smana: 9; sexp: 125; sfail: 50; learned:  False),
			 (sname: 'Frost Ball'; slevel: 19; smana: 12; sexp: 150; sfail: 52; learned:  False),
			 (sname: 'Recharge Item II'; slevel: 21; smana: 12; sexp: 175; sfail: 85; learned:  False),
			 (sname: 'Teleport Other'; slevel: 23; smana: 12; sexp: 200; sfail: 60; learned:  False),
			 (sname: 'Haste Self'; slevel: 25; smana: 12; sexp: 250; sfail: 60; learned:  False),
			 (sname: 'Fire Ball'; slevel: 29; smana: 18; sexp: 350; sfail: 60; learned:  False),
			 (sname: 'Word of Destruction'; slevel: 33; smana: 21; sexp: 500; sfail: 75; learned:  False),
			 (sname: 'Genocide'; slevel: 37; smana: 25; sexp: 800; sfail: 95; learned:   False)),

			{ Priest }
		   ((sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)),

			{ Rogue }
		   ((sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)),

			{ Ranger }
		   ((sname: 'Magic Missle'; slevel: 3; smana: 1; sexp: 6; sfail: 30; learned:  False),
			(sname: 'Detect Monsters'; slevel: 3; smana: 2; sexp: 6; sfail: 35; learned:  False),
			(sname: 'Phase Door'; slevel: 3; smana: 2; sexp: 8; sfail: 35; learned:  False),
			(sname: 'Light Area'; slevel: 5; smana: 3; sexp: 8; sfail: 35; learned:  False),
			(sname: 'Cure Light Wounds'; slevel: 5; smana: 3; sexp: 8; sfail: 40; learned:  False),
			(sname: 'Find Hidden Traps/Doors'; slevel: 5; smana: 4; sexp: 10; sfail: 45; learned:  False),
			(sname: 'Stinking Cloud'; slevel: 7; smana: 6; sexp: 24; sfail: 40; learned:  False),
			(sname: 'Confusion'; slevel: 7; smana: 6; sexp: 20; sfail: 40; learned:  False),
			(sname: 'Lightning Bolt'; slevel: 9; smana: 7; sexp: 30; sfail: 40; learned:  False),
			(sname: 'Trap/Door Destruction'; slevel: 9; smana: 8; sexp: 30; sfail: 45; learned:  False),
			(sname: 'Sleep I'; slevel: 11; smana: 8; sexp: 40; sfail: 40; learned:  False),
			(sname: 'Cure Poison'; slevel: 11; smana: 9; sexp: 40; sfail: 45; learned:  False),
			(sname: 'Teleport Self'; slevel: 13; smana: 10; sexp: 50; sfail: 45; learned:  False),
			(sname: 'Remove Curse'; slevel: 13; smana: 11; sexp: 50; sfail: 55; learned:  False),
			(sname: 'Frost Bolt'; slevel: 15; smana: 12; sexp: 60; sfail: 50; learned:  False),
			(sname: 'Turn Stone to Mud'; slevel: 15; smana: 13; sexp: 60; sfail: 50; learned:  False),
			(sname: 'Identify'; slevel: 17; smana: 25; sexp: 80; sfail: 95; learned:  False),
			(sname: 'Recharge Item I'; slevel: 17; smana: 17; sexp: 70; sfail: 90; learned:  False),
			(sname: 'Sleep II'; slevel: 21; smana: 17; sexp: 70; sfail: 55; learned:  False),
			(sname: 'Polymorph Other'; slevel: 21; smana: 19; sexp: 70; sfail: 60; learned:  False),
			(sname: 'Create Food'; slevel: 23; smana: 17; sexp: 60; sfail: 55; learned:  False),
			(sname: 'Sleep III'; slevel: 23; smana: 20; sexp: 80; sfail: 60; learned:  False),
			(sname: 'Fire Bolt'; slevel: 25; smana: 20; sexp: 80; sfail: 60; learned:  False),
			(sname: 'Slow Monster'; slevel: 25; smana: 21; sexp: 80; sfail: 65; learned:  False),
			(sname: 'Frost Ball'; slevel: 27; smana: 21; sexp: 90; sfail: 65; learned:  False),
			(sname: 'Recharge Item II'; slevel: 29; smana: 23; sexp: 90; sfail: 95; learned:  False),
			(sname: 'Teleport Other'; slevel: 31; smana: 25; sexp: 100; sfail: 70; learned:  False),
			(sname: 'Haste Self'; slevel: 33; smana: 25; sexp: 150; sfail: 75; learned:  False),
			(sname: 'Fire Ball'; slevel: 35; smana: 25; sexp: 200; sfail: 80; learned:  False),
			(sname: 'Word of Destruction'; slevel: 37; smana: 30; sexp: 500; sfail: 95; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)),

			{ Paladin }
		   ((sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)),

			{Consultant}
		   ((sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)),

			{Consultant}
		   ((sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			(sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)));

	priest_spell : {[psect(player$data)]} array [1..MAX_CLASS] of array [1..31] of spell_type = (
			{ Warrior }
			((sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)),

			{ Mage }
			((sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)),

			{ Priest }
			((sname: 'Detect Evil'; slevel: 1; smana: 1; sexp: 3; sfail: 15; learned:  False),
			 (sname: 'Cure Light Wounds'; slevel: 1; smana: 2; sexp: 4; sfail: 25; learned:  False),
			 (sname: 'Bless'; slevel: 1; smana: 2; sexp: 3; sfail: 25; learned:  False),
			 (sname: 'Remove Fear'; slevel: 1; smana: 2; sexp: 3; sfail: 25; learned:  False),
			 (sname: 'Call Light'; slevel: 3; smana: 2; sexp: 6; sfail: 25; learned:  False),
			 (sname: 'Find Traps'; slevel: 3; smana: 3; sexp: 8; sfail: 30; learned:  False),
			 (sname: 'Detect Doors/Stairs'; slevel: 3; smana: 3; sexp: 8; sfail: 30; learned:  False),
			 (sname: 'Slow Poison'; slevel: 3; smana: 3; sexp: 10; sfail: 27; learned:  False),
			 (sname: 'Blind Creature'; slevel: 5; smana: 4; sexp: 16; sfail: 27; learned:  False),
			 (sname: 'Portal'; slevel: 5; smana: 4; sexp: 20; sfail: 27; learned:  False),
			 (sname: 'Cure Medium Wounds'; slevel: 5; smana: 4; sexp: 20; sfail: 30; learned:  False),
			 (sname: 'Chant'; slevel: 5; smana: 5; sexp: 20; sfail: 30; learned:  False),
			 (sname: 'Sanctuary'; slevel: 7; smana: 5; sexp: 30; sfail: 30; learned:  False),
			 (sname: 'Create Food'; slevel: 7; smana: 5; sexp: 30; sfail: 40; learned:  False),
			 (sname: 'Remove Curse'; slevel: 7; smana: 6; sexp: 35; sfail: 40; learned:  False),
			 (sname: 'Resist Heat and Cold'; slevel: 7; smana: 7; sexp: 35; sfail: 40; learned:  False),
			 (sname: 'Neutralize Poison'; slevel: 9; smana: 6; sexp: 40; sfail: 35; learned:  False),
			 (sname: 'Orb of Draining'; slevel: 9; smana: 7; sexp: 40; sfail: 35; learned:  False),
			 (sname: 'Cure Serious Wounds'; slevel: 9; smana: 7; sexp: 40; sfail: 40; learned:  False),
			 (sname: 'Sense Invisible'; slevel: 11; smana: 8; sexp: 40; sfail: 40; learned:  False),
			 (sname: 'Protection from Evil'; slevel: 11; smana: 8; sexp: 50; sfail: 40; learned:  False),
			 (sname: 'Earthquake'; slevel: 11; smana: 9; sexp: 60; sfail: 50; learned:  False),
			 (sname: 'Sense Surroundings'; slevel: 13; smana: 10; sexp: 60; sfail: 45; learned:  False),
			 (sname: 'Cure Critical Wounds'; slevel: 13; smana: 11; sexp: 65; sfail: 45; learned:  False),
			 (sname: 'Turn Undead'; slevel: 15; smana: 12; sexp: 80; sfail: 45; learned:  False),
			 (sname: 'Prayer'; slevel: 15; smana: 14; sexp: 90; sfail: 45; learned:  False),
			 (sname: 'Dispell Undead'; slevel: 17; smana: 14; sexp: 125; sfail: 50; learned:  False),
			 (sname: 'Heal'; slevel: 21; smana: 16; sexp: 150; sfail: 55; learned:  False),
			 (sname: 'Dispell Evil'; slevel: 25; smana: 20; sexp: 300; sfail: 60; learned:  False),
			 (sname: 'Glyph of Warding'; slevel: 33; smana: 24; sexp: 500; sfail: 75; learned:  False),
			 (sname: 'Holy Word'; slevel: 39; smana: 32; sexp: 800; sfail: 95; learned:  False)),

			{ Rogue }
			((sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)),

			{ Ranger }
			((sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)),

			{ Paladin }
			((sname: 'Detect Evil'; slevel: 1; smana: 1; sexp: 4; sfail: 30; learned: False),
			 (sname: 'Cure Light Wounds'; slevel: 2; smana: 2; sexp: 8; sfail: 35; learned: False),
			 (sname: 'Bless'; slevel: 3; smana: 3; sexp: 12; sfail: 35; learned: False),
			 (sname: 'Remove Fear'; slevel: 5; smana: 3; sexp: 20; sfail: 35; learned: False),
			 (sname: 'Call Light'; slevel: 5; smana: 4; sexp: 20; sfail: 35; learned: False),
			 (sname: 'Find Traps'; slevel: 7; smana: 5; sexp: 25; sfail: 40; learned: False),
			 (sname: 'Detect Doors/Stairs'; slevel: 7; smana: 5; sexp: 25; sfail: 40; learned: False),
			 (sname: 'Slow Poison'; slevel: 9; smana: 7; sexp: 30; sfail: 40; learned: False),
			 (sname: 'Blind Creature'; slevel: 9; smana: 7; sexp: 30; sfail: 40; learned: False),
			 (sname: 'Portal'; slevel: 9; smana: 8; sexp: 30; sfail: 40; learned: False),
			 (sname: 'Cure Medium Wounds'; slevel: 11; smana: 9; sexp: 40; sfail: 40; learned: False),
			 (sname: 'Chant'; slevel: 11; smana: 10; sexp: 40; sfail: 45; learned: False),
			 (sname: 'Sanctuary'; slevel: 11; smana: 10; sexp: 40; sfail: 45; learned: False),
			 (sname: 'Create Food'; slevel: 13; smana: 10; sexp: 50; sfail: 45; learned: False),
			 (sname: 'Remove Curse'; slevel: 13; smana: 11; sexp: 50; sfail: 45; learned: False),
			 (sname: 'Resist Heat and Cold'; slevel: 15; smana: 13; sexp: 60; sfail: 45; learned: False),
			 (sname: 'Neutralize Poison'; slevel: 15; smana: 15; sexp: 60; sfail: 50; learned: False),
			 (sname: 'Orb of Draining'; slevel: 17; smana: 15; sexp: 70; sfail: 50; learned: False),
			 (sname: 'Cure Serious Wounds'; slevel: 17; smana: 15; sexp: 70; sfail: 50; learned: False),
			 (sname: 'Sense Invisible'; slevel: 19; smana: 15; sexp: 75; sfail: 50; learned: False),
			 (sname: 'Protection from Evil'; slevel: 19; smana: 15; sexp: 75; sfail: 50; learned: False),
			 (sname: 'Earthquake'; slevel: 21; smana: 17; sexp: 80; sfail: 50; learned: False),
			 (sname: 'Sense Surroundings'; slevel: 23; smana: 17; sexp: 80; sfail: 50; learned: False),
			 (sname: 'Cure Critical Wounds'; slevel: 25; smana: 20; sexp: 80; sfail: 50; learned: False),
			 (sname: 'Turn Undead'; slevel: 27; smana: 21; sexp: 90; sfail: 50; learned: False),
			 (sname: 'Prayer'; slevel: 29; smana: 22; sexp: 95; sfail: 50; learned: False),
			 (sname: 'Dispell Undead'; slevel: 31; smana: 24; sexp: 100; sfail: 60; learned: False),
			 (sname: 'Heal'; slevel: 33; smana: 28; sexp: 125; sfail: 60; learned: False),
			 (sname: 'Dispell Evil'; slevel: 35; smana: 32; sexp: 150; sfail: 70; learned: False),
			 (sname: 'Glyph of Warding'; slevel: 37; smana: 36; sexp: 200; sfail: 90; learned: False),
			 (sname: 'Holy Word'; slevel: 39; smana: 38; sexp: 500; sfail: 95; learned: False)),

			{Consultant}
			((sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)),

			{Consultant}
			((sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)));

	extra_spell : {[psect(player$data)]} array [1..MAX_CLASS] of array [1..31] of spell_type = (
			{Warrior}
			((sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)),
			{Mage}
			((sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)),
			{Priest}
			((sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)),
			{Rogue}
			((sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)),
			{Ranger}
			((sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)),
			{Paladin}
			((sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False),
			 (sname: ''; slevel: 99; smana: 99; sexp: 0; sfail: 0; learned:  False)),
			{Consultant}
			((sname: 'Magic Missle'; slevel: 1; smana: 1; sexp: 5; sfail: 22; learned:  False),
			 (sname: 'Detect Monsters'; slevel: 1; smana: 1; sexp: 5; sfail: 27; learned:  False),
			 (sname: 'Phase Door'; slevel: 1; smana: 2; sexp: 4; sfail: 22; learned:  False),
			 (sname: 'Light Area'; slevel: 1; smana: 2; sexp: 4; sfail: 25; learned:  False),
			 (sname: 'Cure Light Wounds'; slevel: 3; smana: 3; sexp: 8; sfail: 25; learned:  False),
			 (sname: 'Find Hidden Traps/Doors'; slevel: 3; smana: 3; sexp: 6; sfail: 45; learned:  False),
			 (sname: 'Stinking Cloud'; slevel: 3; smana: 4; sexp: 8; sfail: 27; learned:  False),
			 (sname: 'Confusion'; slevel: 3; smana: 4; sexp: 6; sfail: 32; learned:  False),
			 (sname: 'Lightning Bolt'; slevel: 5; smana: 4; sexp: 25; sfail: 32; learned:  False),
			 (sname: 'Trap/Door Destruction'; slevel: 5; smana: 5; sexp: 30; sfail: 37; learned:  False),
			 (sname: 'Sleep I'; slevel: 5; smana: 5; sexp: 20; sfail: 32; learned:  False),
			 (sname: 'Cure Poison'; slevel: 5; smana: 5; sexp: 25; sfail: 37; learned:  False),
			 (sname: 'Teleport Self'; slevel: 7; smana: 6; sexp: 35; sfail: 37; learned:  False),
			 (sname: 'Remove Curse'; slevel: 7; smana: 6; sexp: 40; sfail: 50; learned:  False),
			 (sname: 'Frost Bolt'; slevel: 7; smana: 6; sexp: 50; sfail: 40; learned:  False),
			 (sname: 'Turn Stone to Mud'; slevel: 9; smana: 7; sexp: 75; sfail: 44; learned:  False),
			 (sname: 'Identify'; slevel: 9; smana: 7; sexp: 75; sfail: 95; learned:  False),
			 (sname: 'Recharge Item I'; slevel: 9; smana: 7; sexp: 90; sfail: 70; learned:  False),
			 (sname: 'Sleep II'; slevel: 9; smana: 7; sexp: 75; sfail: 45; learned:  False),
			 (sname: 'Polymorph Other'; slevel: 11; smana: 7; sexp: 100; sfail: 45; learned:  False),
			 (sname: 'Create Food'; slevel: 11; smana: 7; sexp: 75; sfail: 50; learned:  False),
			 (sname: 'Sleep III'; slevel: 13; smana: 7; sexp: 90; sfail: 50; learned:  False),
			 (sname: 'Fire Bolt'; slevel: 15; smana: 9; sexp: 100; sfail: 50; learned:  False),
			 (sname: 'Slow Monster'; slevel: 17; smana: 9; sexp: 125; sfail: 50; learned:  False),
			 (sname: 'Frost Ball'; slevel: 19; smana: 12; sexp: 150; sfail: 52; learned:  False),
			 (sname: 'Recharge Item II'; slevel: 21; smana: 12; sexp: 175; sfail: 85; learned:  False),
			 (sname: 'Teleport Other'; slevel: 23; smana: 12; sexp: 200; sfail: 60; learned:  False),
			 (sname: 'Haste Self'; slevel: 25; smana: 12; sexp: 250; sfail: 60; learned:  False),
			 (sname: 'Fire Ball'; slevel: 29; smana: 18; sexp: 350; sfail: 60; learned:  False),
			 (sname: 'Word of Destruction'; slevel: 33; smana: 21; sexp: 500; sfail: 75; learned:  False),
			 (sname: 'Genocide'; slevel: 37; smana: 25; sexp: 800; sfail: 95; learned:   False)),
			{Consultant}
			((sname: 'Magic Missle'; slevel: 1; smana: 1; sexp: 5; sfail: 22; learned:  False),
			 (sname: 'Detect Monsters'; slevel: 1; smana: 1; sexp: 5; sfail: 27; learned:  False),
			 (sname: 'Phase Door'; slevel: 1; smana: 2; sexp: 4; sfail: 22; learned:  False),
			 (sname: 'Light Area'; slevel: 1; smana: 2; sexp: 4; sfail: 25; learned:  False),
			 (sname: 'Cure Light Wounds'; slevel: 3; smana: 3; sexp: 8; sfail: 25; learned:  False),
			 (sname: 'Find Hidden Traps/Doors'; slevel: 3; smana: 3; sexp: 6; sfail: 45; learned:  False),
			 (sname: 'Stinking Cloud'; slevel: 3; smana: 4; sexp: 8; sfail: 27; learned:  False),
			 (sname: 'Confusion'; slevel: 3; smana: 4; sexp: 6; sfail: 32; learned:  False),
			 (sname: 'Lightning Bolt'; slevel: 5; smana: 4; sexp: 25; sfail: 32; learned:  False),
			 (sname: 'Trap/Door Destruction'; slevel: 5; smana: 5; sexp: 30; sfail: 37; learned:  False),
			 (sname: 'Sleep I'; slevel: 5; smana: 5; sexp: 20; sfail: 32; learned:  False),
			 (sname: 'Cure Poison'; slevel: 5; smana: 5; sexp: 25; sfail: 37; learned:  False),
			 (sname: 'Teleport Self'; slevel: 7; smana: 6; sexp: 35; sfail: 37; learned:  False),
			 (sname: 'Remove Curse'; slevel: 7; smana: 6; sexp: 40; sfail: 50; learned:  False),
			 (sname: 'Frost Bolt'; slevel: 7; smana: 6; sexp: 50; sfail: 40; learned:  False),
			 (sname: 'Turn Stone to Mud'; slevel: 9; smana: 7; sexp: 75; sfail: 44; learned:  False),
			 (sname: 'Identify'; slevel: 9; smana: 7; sexp: 75; sfail: 95; learned:  False),
			 (sname: 'Recharge Item I'; slevel: 9; smana: 7; sexp: 90; sfail: 70; learned:  False),
			 (sname: 'Sleep II'; slevel: 9; smana: 7; sexp: 75; sfail: 45; learned:  False),
			 (sname: 'Polymorph Other'; slevel: 11; smana: 7; sexp: 100; sfail: 45; learned:  False),
			 (sname: 'Create Food'; slevel: 11; smana: 7; sexp: 75; sfail: 50; learned:  False),
			 (sname: 'Sleep III'; slevel: 13; smana: 7; sexp: 90; sfail: 50; learned:  False),
			 (sname: 'Fire Bolt'; slevel: 15; smana: 9; sexp: 100; sfail: 50; learned:  False),
			 (sname: 'Slow Monster'; slevel: 17; smana: 9; sexp: 125; sfail: 50; learned:  False),
			 (sname: 'Frost Ball'; slevel: 19; smana: 12; sexp: 150; sfail: 52; learned:  False),
			 (sname: 'Recharge Item II'; slevel: 21; smana: 12; sexp: 175; sfail: 85; learned:  False),
			 (sname: 'Teleport Other'; slevel: 23; smana: 12; sexp: 200; sfail: 60; learned:  False),
			 (sname: 'Haste Self'; slevel: 25; smana: 12; sexp: 250; sfail: 60; learned:  False),
			 (sname: 'Fire Ball'; slevel: 29; smana: 18; sexp: 350; sfail: 60; learned:  False),
			 (sname: 'Word of Destruction'; slevel: 33; smana: 21; sexp: 500; sfail: 75; learned:  False),
			 (sname: 'Genocide'; slevel: 37; smana: 25; sexp: 800; sfail: 95; learned:   False)));

{ Following are treasure arrays and variables   }

{ Object description:  Objects are defined here.  Each object has the following
					   attributes:
	  Descriptor : Name of item and formats.
				   %C is replaced with a random color.
				   %W is replaced with a random wood.
				   %M is replaced with a random metal.
				   %R is replaced with a random rock.
				   %P1 is replaced with the number in P1 (see below).
				   %P2 is replaced with plus-to-hit.
				   %P3 is replaced with plus-to-damage.
				   %P4 is replaced with plus-to-AC.
				   %P5 is replaced with number in P1, no sign.
				   & is replaced with 'a', 'an', or a number.
				   ~ is replaced with null or 's'.
	  Character  : Character that represents the item.
	  Type value : Value representing the type of object.
				   1   ~   Miscellaneous Object.
				   2   &   chest.
				   10  (Left Curly Brace)  sling ammo.
				   11  (Left Curly Brace)  bolt.
				   12  (Left Curly Brace)  arrow.
				   13  ~   spike.
				   15  ~   Lamp or Torch.
				   20  (Right curly brace)  bow, crossbow, or sling.
				   21  /   hafted weapon.
				   22  \   pole arm.
				   23  |   sword or dagger.
				   25  \   digging tools.
				   30  ]   boots.
				   31  ]   gloves and gauntlets.
				   32  (   Cloak
				   33  ]   helm.
				   34  )   shield.
				   35  [   hard armor.
				   36  (   soft armor.
				   40  "   amulet.
				   45  =   ring.
				   55  _   staff.
				   60  -   rod.
				   65  -   wand.
				   70  ?   scroll.
				   71  ?   scroll.    continued
				   75  !   potion.
				   76  !   potion.    continued
				   77  !   flask of oil.
				   80  ,   food.
				   90  ?   Magic Book.
				   91  ?   Prayer Book.
				   92  ?   extra book 1.
				   93  ?   extra book 2.
				   100 $   valuable metal.
				   100 *   valuable gems.
				   101 .   Unseen trap.
				   102 ^   Seen trap.
				   103 :   Rubble.
				   104 '   Open door.
				   105 +   Closed door.
				   107 <   Up staircase.
				   108 >   Down staircase.
				   109 #   Secret door.
				   110 1,2,3,4,5,6,7   Entrance to store.
				   111 ^   Black Market Trap (store)

	  Sub value  : separate value for each item of a type.
	  Damage     : amount of damage item can cause.
	  Weight     : relative weight of an item.
	  Number     : number of items appearing in group.
				   If items can be added together then subval > 255
				   If items are considered one unit when added together then
								subval > 511
	  To hit     : magical pluses to hit.
	  To damage  : magical pluses to damage.
	  AC         : objects relative armor class.
				   1 is worse than 5 is worse than 10 ect...
	  To AC      : Magical bonuses to AC.
	  P1         : Catch all for magical abilities such as pluses to strength,
				   minuses to searching...
	  Flags      : Abilities of object.  Each abilitiy is a bit.  Bits 1-31
				   are used. (Signed integer)

	  Flags used for anything that can be worn or wielded (not eaten/drunk):
					  Bit #   Hex #           Ability
					  1       00000001        Strength
					  2       00000002        Dexterity
					  3       00000004        Constitution
					  4       00000008        Intelligence
					  5       00000010        Wisdom
					  6       00000020        Charisma
					  7       00000040        Searching
					  8       00000080        Slow digestion
					  9       00000100        Stealth
					  10      00000200        Aggravation
					  11      00000400        Teleportation
					  12      00000800        Regeneration
					  13      00001000        Speed
					  14      00002000        Slay Dragon
					  15      00004000        Slay Monster
					  16      00008000        Slay Evil
					  17      00010000        Slay Undead
					  18      00020000        Cold Brand
					  19      00040000        Flame Brand
					  20      00080000        Resist Fire
					  21      00100000        Resist Acid
					  22      00200000        Resist Cold
					  23      00400000        Sustain Stat (P1)
					  24      00800000        Free Action
					  25      01000000        See Invisible
					  26      02000000        Resist Lightning
					  27      04000000        Feather Fall
					  28      08000000        Blindness
					  29      10000000        Timidness
					  30      20000000        Tunneling
					  31      40000000        Infra vision
					  32      80000000        Cursed

	  Flags used for anything that can be eaten/drunk:
					  Bit #   Hex #           Ability
					  1       00000001        Poison
					  2       00000002        Blindness
					  3       00000004        Fear
					  4       00000008        Confusion
					  5       00000010        Hallucination
					  6       00000020        Cure Poison
					  7       00000040        Cure Blindness
					  8       00000080        Cure Fear
					  9       00000100        Cure Confusion
					  10      00000200        Lose Strength
					  11      00000400        Lose Constitution
					  12      00000800        Lose Intelligence
					  13      00001000        Lose Wisdom
					  14      00002000        Lose Dexterity
					  15      00004000        Lose Charisma
					  16      00008000        Restore Strength
					  17      00010000        Restore Constitution
					  18      00020000        Restore Intelligence
					  19      00040000        Restore Wisdom
					  20      00080000        Restore Dexterity
					  21      00100000        Restore Charisma
					  22      00200000        Poisonous Food 1d3
					  23      00400000        Poisonous Food 1d6
					  24      00800000        Poisonous Food 1d12
					  25      01000000        Poisonous Food 3d6
					  26      02000000        Poisonous Food 3d12
					  27      04000000        Poisonous Food 1d4
					  28      08000000        Poisonous Food 1d8
					  29      10000000        Poisonous Food 2d8
					  30      20000000        Poisonous Food 3d8
					  31      40000000        Poisonous Food 1d1
					  32      80000000        -

	  Level      : Minimum level on which item can be found.
	  Cost       : Relative cost of item.
				   Special Abilities can be added to item by MAGIC_INIT,
				   found in MISC.INC.

		Scrolls, Potions, and Food:
				Flags is used to define a function which reading/quaffing
				will cause.  Most scrolls and potions have only one bit
				set.  Potions will generally have some food value, found
				in P1.

		Wands and Staves:
				Flags defines a function, P1 contains number of charges
				for item.  P1 is set in MAGIC_INIT (MISC.INC)

		Chests:
				Traps are added randomly by MAGIC.INIT in MISC.INC.

		  Object list (All objects must be defined here) }

	object_list : {[psect(moria$data)]} array [1..MAX_OBJECTS] of treasure_type = (
			(name: '& %M Mushroom~| of Poison';
					tval: 80; tchar: ','; flags: $00000001; p1: 500; cost: 0;
					subval: 257; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 7),
			(name: '& %M Mushroom~| of Blindness';
					tval: 80; tchar: ','; flags: $00000002; p1: 500; cost: 0;
					subval: 258; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0 '; level: 9),
			(name: '& %M Mushroom~| of Paranoia';
					tval: 80; tchar: ','; flags: $00000004; p1: 500; cost: 0;
					subval: 259; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 9),
			(name: '& %M Mushroom~| of Confusion';
					tval: 80; tchar: ','; flags: $00000008; p1: 500; cost: 0;
					subval: 260; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 7),
			(name: '& %M Mushroom~| of Halucination';
					tval: 80; tchar: ','; flags: $00000010; p1: 500; cost: 0;
					subval: 261; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 13),
			(name: '& %M Mushroom~| of Cure Poison';
					tval: 80; tchar: ','; flags: $00000020; p1: 500; cost: 60;
					subval: 262; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 8),
			(name: '& %M Mushroom~| of Cure Blindness';
					tval: 80; tchar: ','; flags: $00000040; p1: 500; cost: 50;
					subval: 263; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 10),
			(name: '& %M Mushroom~| of Cure Paranoia';
					tval: 80; tchar: ','; flags: $00000080; p1: 500; cost: 25;
					subval: 264; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 12),
			(name: '& %M Mushroom~| of Cure Confusion';
					tval: 80; tchar: ','; flags: $00000100; p1: 500; cost: 50;
					subval: 265; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 6),
			(name: '& %M Mushroom~| of Weakness';
					tval: 80; tchar: ','; flags: $04000200; p1: 500; cost: 0;
					subval: 266; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 7),
			(name: '& %M Mushroom~| of Unhealth';
					tval: 80; tchar: ','; flags: $04000400; p1: 500; cost: 50;
					subval: 267; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '10d10'; level: 15),
			(name: '& %M Mushroom~| of Restore Constitution';
					tval: 80; tchar: ','; flags: $00010000; p1: 500; cost: 350;
					subval: 268; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 20),
			(name: '& %M Mushroom~| of First-Aid';
					tval: 80; tchar: ','; flags: $00200000; p1: 500; cost: 5;
					subval: 269; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 6),
			(name: '& %M Mushroom~| of Minor Cures';
					tval: 80; tchar: ','; flags: $00400000; p1: 500; cost: 20;
					subval: 270; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 7),
			(name: '& %M Mushroom~| of Light Cures';
					tval: 80; tchar: ','; flags: $00800000; p1: 500; cost: 30;
					subval: 271; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 10),
			(name: '& %M Mushroom~| of Restoring ';
					tval: 80; tchar: ','; flags: $001F8040; p1: 500; cost: 1000;
					subval: 272; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 30),
			(name: '& Hairy %M Mold~| of Poison';
					tval: 80; tchar: ','; flags: $00000001; p1: 1200; cost: 0;
					subval: 273; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 15),
			(name: '& Hairy %M Mold~| of Halucinations';
					tval: 80; tchar: ','; flags: $00000010; p1: 1200; cost: 0;
					subval: 274; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 18),
			(name: '& Hairy %M Mold~| of Cure Poison';
					tval: 80; tchar: ','; flags: $00000020; p1: 1200; cost: 75;
					subval: 275; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 19),
			(name: '& Hairy %M Mold~| of Unhealth';
					tval: 80; tchar: ','; flags: $00000400; p1: 1200; cost: 25;
					subval: 276; weight: 1; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '6d8'; level: 28),
			(name: '& Hairy %M Mold~| of Cure Serious Wounds';
					tval: 80; tchar: ','; flags: $02000000; p1: 1200; cost: 75;
					subval: 277; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 16),
//dengland  Food is consumed far too quickly.  Replacing the following values.
//			(name: '& Ration~ of Food';
//					tval: 80; tchar: ','; flags: $00000000; p1: 5000; cost: 3;
//					subval: 307; weight: 10; number: 1; tohit: 0; todam: 0;
//					ac: 0; toac: 0; damage: '0d0'; level: 0),
			(name: '& Ration~ of Food';
					tval: 80; tchar: ','; flags: $00000000; p1: PLAYER_FOOD_FULL; cost: 3;
					subval: 307; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),
			(name: '& Ration~ of Food';
					tval: 80; tchar: ','; flags: $00000000; p1: PLAYER_FOOD_FULL; cost: 3;
					subval: 307; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 5),
			(name: '& Ration~ of Food';
					tval: 80; tchar: ','; flags: $00000000; p1: PLAYER_FOOD_FULL; cost: 3;
					subval: 307; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 10),
//			(name: '& Slime Mold~';
//					tval: 80; tchar: ','; flags: $00000000; p1: 3000; cost: 2;
//					subval: 308; weight: 5; number: 1; tohit: 0; todam: 0; /
//					ac: 0; toac: 0; damage: '0d0'; level: 1),
			(name: '& Slime Mold~';
					tval: 80; tchar: ','; flags: $00000000; p1: 5000; cost: 2;
					subval: 308; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 1),
//			(name: '& Piece~ of Elvish Waybread';
//					tval: 80; tchar: ','; flags: $02000020; p1: 7500; cost: 10;
//					subval: 311; weight: 3; number: 1; tohit: 0; todam: 0;
//					ac: 0; toac: 0; damage: '0d0'; level: 6),
			(name: '& Piece~ of Elvish Waybread';
					tval: 80; tchar: ','; flags: $02000020; p1: PLAYER_FOOD_MAX; cost: 10;
					subval: 311; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 6),
			(name: '& Piece~ of Elvish Waybread';
					tval: 80; tchar: ','; flags: $02000020; p1: PLAYER_FOOD_MAX; cost: 10;
					subval: 311; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 12),
			(name: '& Piece~ of Elvish Waybread';
					tval: 80; tchar: ','; flags: $02000020; p1: PLAYER_FOOD_MAX; cost: 10;
					subval: 311; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 20),
			(name: '& Dagger <Main Gauche>^ (%P2,%P3)';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 25;
					subval: 1; weight: 30; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d5'; level: 2),
			(name: '& Dagger <Misercorde>^ (%P2,%P3)';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 10;
					subval: 2; weight: 15; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d4'; level: 0),
			(name: '& Dagger <Stiletto>^ (%P2,%P3)';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 10;
					subval: 3; weight: 12; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d4'; level: 0),
			(name: '& Dagger <Bodkin>^ (%P2,%P3) ';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 10;
					subval: 4; weight: 20; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d4'; level: 1),
			(name: '& Broken dagger^ (%P2,%P3) ';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 0;
					subval: 5; weight: 15; number: 1; tohit: -2; todam: -2;
					ac: 0; toac: 0; damage: '1d1'; level: 0),
			(name: '& Backsword^ (%P2,%P3)';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 60;
					subval: 6; weight: 95; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d9'; level: 7),
			(name: '& Bastard Sword^ (%P2,%P3) ';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 350;
					subval: 7; weight: 140; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d4'; level: 14),
			(name: '& Thrusting Sword <Bilbo>^ (%P2,%P3) ';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 60;
					subval: 8; weight: 80; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d6'; level: 4),
			(name: '& Thrusting Sword <Baselard>^ (%P2,%P3) ';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 80;
					subval: 9; weight: 100; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d7'; level: 5),
			(name: '& Broadsword^ (%P2,%P3) ';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 255;
					subval: 10; weight: 150; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d5'; level: 9),
			(name: '& Two Handed Sword <Claymore>^ (%P2,%P3)';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 775;
					subval: 11; weight: 200; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d6'; level: 30),
			(name: '& Cutlass^ (%P2,%P3) ';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 85;
					subval: 12; weight: 110; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d7'; level: 7),
			(name: '& Two Handed Sword <Espadon>^ (%P2,%P3) ';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 655;
					subval: 13; weight: 180; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d6'; level: 35),
			(name: '& Executioner''s Sword^ (%P2,%P3)';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 850;
					subval: 14; weight: 260; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '4d5'; level: 40),
			(name: '& Two Handed Sword <Flamberge>^ (%P2,%P3)';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 1000;
					subval: 15; weight: 240; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '4d5'; level: 45),
			(name: '& Foil^ (%P2,%P3)';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 35;
					subval: 16; weight: 30; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d5'; level: 2),
			(name: '& Katana^ (%P2,%P3)';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 400;
					subval: 17; weight: 120; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d4'; level: 18),
			(name: '& Longsword^ (%P2,%P3)';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 300;
					subval: 18; weight: 130; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d10 '; level: 12),
			(name: '& Two Handed Sword <No-Dachi>^ (%P2,%P3)';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 675;
					subval: 19; weight: 200; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '4d4'; level: 45),
			(name: '& Rapier^ (%P2,%P3)';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 42;
					subval: 20; weight: 40; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d6'; level: 4),
			(name: '& Sabre^ (%P2,%P3) ';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 50;
					subval: 21; weight: 50; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d7'; level: 5),
			(name: '& Small Sword^ (%P2,%P3)';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 48;
					subval: 22; weight: 75; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d6'; level: 5),
			(name: '& Two Handed Sword <Zweihander>^ (%P2,%P3) ';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 1000;
					subval: 23; weight: 280; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '4d6'; level: 50),
			(name: '& Broken sword^ (%P2,%P3)';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost: 0;
					subval: 24; weight: 75; number: 1; tohit: -2; todam: -2;
					ac: 0; toac: 0; damage: '1d1'; level: 0),
			(name: '& Battle Axe <Balestarius>^ (%P2,%P3)';
					tval: 21; tchar: '\'; flags: $00000000; p1: 0; cost: 500;
					subval: 1; weight: 180; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d8'; level: 30),
			(name: '& Ball and Chain^ (%P2,%P3)';
					tval: 21; tchar: '\'; flags: $00000000; p1: 0; cost: 200;
					subval: 2; weight: 150; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d4'; level: 20),
			(name: '& Battle Axe <European>^ (%P2,%P3) ';
					tval: 21; tchar: '\'; flags: $00000000; p1: 0; cost: 334;
					subval: 3; weight: 170; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d4'; level: 13),
			(name: '& Broad Axe^ (%P2,%P3)';
					tval: 21; tchar: '\'; flags: $00000000; p1: 0; cost: 304;
					subval: 4; weight: 160; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d6'; level: 17),
			(name: '& Cat-O-Nine Tails^ (%P2,%P3)';
					tval: 21; tchar: '\'; flags: $00000000; p1: 0; cost: 14;
					subval: 5; weight: 40; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d4'; level: 3),
			(name: '& Wooden Club^ (%P2,%P3)';
					tval: 21; tchar: '\'; flags: $00000000; p1: 0; cost: 1;
					subval: 6; weight: 100; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d3'; level: 0),
			(name: '& Flail^ (%P2,%P3) ';
					tval: 21; tchar: '\'; flags: $00000000; p1: 0; cost: 353;
					subval: 7; weight: 150; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d6'; level: 12),
			(name: '& Two Handed Great Flail^ (%P2,%P3)';
					tval: 21; tchar: '\'; flags: $00000000; p1: 0; cost: 590;
					subval: 8; weight: 280; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d6'; level: 45),
			(name: '& Morningstar^ (%P2,%P3)';
					tval: 21; tchar: '\'; flags: $00000000; p1: 0; cost: 396;
					subval: 9; weight: 150; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d6'; level: 10),
			(name: '& Mace^ (%P2,%P3)';
					tval: 21; tchar: '\'; flags: $00000000; p1: 0; cost: 130;
					subval: 10; weight: 120; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d4'; level: 6),
			(name: '& War Hammer^ (%P2,%P3) ';
					tval: 21; tchar: '\'; flags: $00000000; p1: 0; cost: 225;
					subval: 11; weight: 120; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d3'; level: 5),
			(name: '& Mace <Lead filled>^ (%P2,%P3) ';
					tval: 21; tchar: '\'; flags: $00000000; p1: 0; cost: 502;
					subval: 12; weight: 180; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d4'; level: 15),
			(name: '& Awl-Pike^ (%P2,%P3)';
					tval: 22; tchar: '/'; flags: $00000000; p1: 0; cost: 340;
					subval: 1; weight: 160; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d8'; level: 8),
			(name: '& Beaked Axe^ (%P2,%P3) ';
					tval: 22; tchar: '/'; flags: $00000000; p1: 0; cost: 408;
					subval: 2; weight: 180; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d6'; level: 15),
			(name: '& Fauchard^ (%P2,%P3)';
					tval: 22; tchar: '/'; flags: $00000000; p1: 0; cost: 376;
					subval: 3; weight: 170; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d10 '; level: 17),
			(name: '& Glaive^ (%P2,%P3)';
					tval: 22; tchar: '/'; flags: $00000000; p1: 0; cost: 363;
					subval: 4; weight: 190; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d6'; level: 20),
			(name: '& Halberd^ (%P2,%P3) ';
					tval: 22; tchar: '/'; flags: $00000000; p1: 0; cost: 430;
					subval: 5; weight: 190; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d4'; level: 22),
			(name: '& Lucerne Hammer^ (%P2,%P3)';
					tval: 22; tchar: '/'; flags: $00000000; p1: 0; cost: 376;
					subval: 6; weight: 120; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d5'; level: 11),
			(name: '& Pike^ (%P2,%P3)';
					tval: 22; tchar: '/'; flags: $00000000; p1: 0; cost: 358;
					subval: 7; weight: 160; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d5'; level: 15),
			(name: '& Spear^ (%P2,%P3) ';
					tval: 22; tchar: '/'; flags: $00000000; p1: 0; cost: 36;
					subval: 8; weight: 50; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d6'; level: 5),
			(name: '& Lance^ (%P2,%P3) ';
					tval: 22; tchar: '/'; flags: $00000000; p1: 0; cost: 230;
					subval: 9; weight: 300; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d8'; level: 10),
			(name: '& Javelin^ (%P2,%P3) ';
					tval: 22; tchar: '/'; flags: $00000000; p1: 0; cost: 18;
					subval: 10; weight: 30; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d4'; level: 4),
			(name: '& Short Bow^ (%P2) ';
					tval: 20; tchar: '}'; flags: $00000000; p1: 2; cost: 50;
					subval: 1; weight: 30; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 3),
			(name: '& Long Bow^ (%P2)';
					tval: 20; tchar: '}'; flags: $00000000; p1: 3; cost: 120;
					subval: 2; weight: 40; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 10),
			(name: '& Composite Bow^ (%P2)';
					tval: 20; tchar: '}'; flags: $00000000; p1: 4; cost: 240;
					subval: 3; weight: 40; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 40),
			(name: '& Light Crossbow^ (%P2) ';
					tval: 20; tchar: '}'; flags: $00000000; p1: 5; cost: 140;
					subval: 10; weight: 110; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 15),
			(name: '& Heavy Crossbow^ (%P2) ';
					tval: 20; tchar: '}'; flags: $00000000; p1: 6; cost: 300;
					subval: 11; weight: 200; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 30),
			(name: '& Sling^ (%P2)';
					tval: 20; tchar: '}'; flags: $00000000; p1: 1; cost: 5;
					subval: 20; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 1),
			(name: '& Arrow~^ (%P2,%P3)';
					tval: 12; tchar: '{'; flags: $00000000; p1: 0; cost: 1;
					subval: 1; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d4'; level: 2),
			(name: '& Bolt~^ (%P2,%P3) ';
					tval: 11; tchar: '{'; flags: $00000000; p1: 0; cost: 2;
					subval: 1; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d5'; level: 2),
			(name: '& Rounded Pebble~^ (%P2,%P3) ';
					tval: 10; tchar: '{'; flags: $00000000; p1: 0; cost: 1;
					subval: 1; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d2'; level: 0),
			(name: '& Iron Shot~^ (%P2,%P3) ';
					tval: 10; tchar: '{'; flags: $00000000; p1: 0; cost: 2;
					subval: 1; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d3'; level: 3),
			(name: '& Iron Spike~';
					tval: 13; tchar: '~'; flags: $00000000; p1: 0; cost: 1;
					subval: 1; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 1),
			(name: '& Brass Lantern~ with %P5 turns of light';
					tval: 15; tchar: '~'; flags: $00000000; p1: 7500; cost: 35;
					subval: 1; weight: 50; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 1),
			(name: '& Wooden Torch~ with %P5 turns of light ';
					tval: 15; tchar: '~'; flags: $00000000; p1: 4000; cost: 2;
					subval: 13; weight: 30; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 1),
			(name: '& Orcish Pick^ (%P1) (%P2,%P3)';
					tval: 25; tchar: '\'; flags: $20000000; p1: 2; cost: 500;
					subval: 2; weight: 180; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d5'; level: 20),
			(name: '& Dwarven Pick^ (%P1) (%P2,%P3) ';
					tval: 25; tchar: '\'; flags: $20000000; p1: 3; cost: 1200;
					subval: 3; weight: 200; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d5'; level: 50),
			(name: '& Gnomish Shovel^ (%P1) (%P2,%P3)';
					tval: 25; tchar: '\'; flags: $20000000; p1: 1; cost: 100;
					subval: 5; weight: 50; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d2'; level: 20),
			(name: '& Dwarven Shovel^ (%P1) (%P2,%P3)';
					tval: 25; tchar: '\'; flags: $20000000; p1: 2; cost: 250;
					subval: 6; weight: 120; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d3'; level: 40),
			(name: '& Pair of Soft Leather Shoes^ [%P6,%P4] ';
					tval: 30; tchar: ']'; flags: $00000000; p1: 0; cost: 4;
					subval: 1; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 1; toac: 0; damage: '0d0'; level: 1),
			(name: '& Pair of Soft Leather Boots^ [%P6,%P4] ';
					tval: 30; tchar: ']'; flags: $00000000; p1: 0; cost: 7;
					subval: 2; weight: 20; number: 1; tohit: 0; todam: 0;
					ac: 2; toac: 0; damage: '1d1'; level: 4),
			(name: '& Pair of Hard Leather Boots^ [%P6,%P4] ';
					tval: 30; tchar: ']'; flags: $00000000; p1: 0; cost: 12;
					subval: 3; weight: 40; number: 1; tohit: 0; todam: 0;
					ac: 3; toac: 0; damage: '1d1'; level: 6),
			(name: '& Soft Leather Cap^ [%P6,%P4]';
					tval: 33; tchar: ']'; flags: $00000000; p1: 0; cost: 4;
					subval: 1; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 1; toac: 0; damage: '0d0'; level: 2),
			(name: '& Hard Leather Cap^ [%P6,%P4]';
					tval: 33; tchar: ']'; flags: $00000000; p1: 0; cost: 12;
					subval: 2; weight: 15; number: 1; tohit: 0; todam: 0;
					ac: 2; toac: 0; damage: '0d0'; level: 4),
			(name: '& Metal Cap^ [%P6,%P4]';
					tval: 33; tchar: ']'; flags: $00000000; p1: 0; cost: 30;
					subval: 3; weight: 20; number: 1; tohit: 0; todam: 0;
					ac: 3; toac: 0; damage: '1d1'; level: 7),
			(name: '& Iron Helm^ [%P6,%P4]';
					tval: 33; tchar: ']'; flags: $00000000; p1: 0; cost: 75;
					subval: 4; weight: 75; number: 1; tohit: 0; todam: 0;
					ac: 5; toac: 0; damage: '1d3'; level: 20),
			(name: '& Steel Helm^ [%P6,%P4] ';
					tval: 33; tchar: ']'; flags: $00000000; p1: 0; cost: 200;
					subval: 5; weight: 60; number: 1; tohit: 0; todam: 0;
					ac: 6; toac: 0; damage: '1d3'; level: 40),
			(name: '& Silver Crown^ (%P1)';
					tval: 33; tchar: ']'; flags: $00000000; p1: 0; cost: 250;
					subval: 6; weight: 20; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 44),
			(name: '& Golden Crown^ (%P1)';
					tval: 33; tchar: ']'; flags: $00000000; p1: 0; cost: 500;
					subval: 7; weight: 30; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 47),
			(name: '& Jewel Encrusted Crown^ (%P1)';
					tval: 33; tchar: ']'; flags: $00000000; p1: 0; cost: 1000;
					subval: 8; weight: 40; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50),
			(name: '& Robe^ [%P6,%P4]';
					tval: 36; tchar: '('; flags: $00000000; p1: 0; cost: 4;
					subval: 1; weight: 20; number: 1; tohit: 0; todam: 0;
					ac: 2; toac: 0; damage: '0d0'; level: 1),
			(name: 'Soft Leather Armor^ [%P6,%P4]';
					tval: 36; tchar: '('; flags: $00000000; p1: 0; cost: 18;
					subval: 2; weight: 80; number: 1; tohit: 0; todam: 0;
					ac: 4; toac: 0; damage: '0d0'; level: 2),
			(name: 'Soft Studded Leather^ [%P6,%P4] ';
					tval: 36; tchar: '('; flags: $00000000; p1: 0; cost: 35;
					subval: 3; weight: 90; number: 1; tohit: 0; todam: 0;
					ac: 5; toac: 0; damage: '1d1'; level: 3),
			(name: 'Hard Leather Armor^ [%P6,%P4]';
					tval: 36; tchar: '('; flags: $00000000; p1: 0; cost: 55;
					subval: 4; weight: 100; number: 1; tohit: -1; todam: 0;
					ac: 6; toac: 0; damage: '1d1'; level: 5),
			(name: 'Hard Studded Leather^ [%P6,%P4] ';
					tval: 36; tchar: '('; flags: $00000000; p1: 0; cost: 100;
					subval: 5; weight: 110; number: 1; tohit: -1; todam: 0;
					ac: 7; toac: 0; damage: '1d2'; level: 7),
			(name: 'Woven Cord Armor^ [%P6,%P4]';
					tval: 36; tchar: '('; flags: $00000000; p1: 0; cost: 45;
					subval: 6; weight: 150; number: 1; tohit: -1; todam: 0;
					ac: 6; toac: 0; damage: '0d0'; level: 7),
			(name: 'Soft Leather Ring Mail^ [%P6,%P4]';
					tval: 36; tchar: '('; flags: $00000000; p1: 0; cost: 160;
					subval: 7; weight: 130; number: 1; tohit: -1; todam: 0;
					ac: 6; toac: 0; damage: '1d2'; level: 10),
			(name: 'Hard Leather Ring Mail^ [%P6,%P4]';
					tval: 36; tchar: '('; flags: $00000000; p1: 0; cost: 230;
					subval: 8; weight: 150; number: 1; tohit: -2; todam: 0;
					ac: 8; toac: 0; damage: '1d3'; level: 12),
			(name: 'Leather Scale Mail^ [%P6,%P4]';
					tval: 36; tchar: '('; flags: $00000000; p1: 0; cost: 330;
					subval: 9; weight: 140; number: 1; tohit: -1; todam: 0;
					ac: 11; toac: 0; damage: '1d1'; level: 14),
			(name: 'Metal Scale Mail^ [%P6,%P4]';
					tval: 35; tchar: '['; flags: $00000000; p1: 0; cost: 430;
					subval: 1; weight: 250; number: 1; tohit: -2; todam: 0;
					ac: 13; toac: 0; damage: '1d4'; level: 24),
			(name: 'Chain Mail^ [%P6,%P4]';
					tval: 35; tchar: '['; flags: $00000000; p1: 0; cost: 530;
					subval: 2; weight: 220; number: 1; tohit: -2; todam: 0;
					ac: 14; toac: 0; damage: '1d4'; level: 26),
			(name: 'Rusty Chain Mail^ [%P6,%P4]';
					tval: 35; tchar: '['; flags: $00000000; p1: 0; cost: 0;
					subval: 3; weight: 200; number: 1; tohit: -5; todam: 0;
					ac: 14; toac: -8; damage: '1d4'; level: 26),
			(name: 'Double Chain Mail^ [%P6,%P4] ';
					tval: 35; tchar: '['; flags: $00000000; p1: 0; cost: 630;
					subval: 4; weight: 260; number: 1; tohit: -2; todam: 0;
					ac: 15; toac: 0; damage: '1d4'; level: 28),
			(name: 'Augmented Chain Mail^ [%P6,%P4] ';
					tval: 35; tchar: '['; flags: $00000000; p1: 0; cost: 675;
					subval: 5; weight: 270; number: 1; tohit: -2; todam: 0;
					ac: 16; toac: 0; damage: '1d4'; level: 30),
			(name: 'Bar Chain Mail^ [%P6,%P4]';
					tval: 35; tchar: '['; flags: $00000000; p1: 0; cost: 720;
					subval: 6; weight: 280; number: 1; tohit: -2; todam: 0;
					ac: 18; toac: 0; damage: '1d4'; level: 34),
			(name: 'Metal Brigandine Armor^ [%P6,%P4]';
					tval: 35; tchar: '['; flags: $00000000; p1: 0; cost: 775;
					subval: 7; weight: 290; number: 1; tohit: -3; todam: 0;
					ac: 19; toac: 0; damage: '1d4'; level: 36),
			(name: 'Laminated Armor^ [%P6,%P4] ';
					tval: 35; tchar: '['; flags: $00000000; p1: 0; cost: 825;
					subval: 8; weight: 300; number: 1; tohit: -3; todam: 0;
					ac: 20; toac: 0; damage: '1d4'; level: 38),
			(name: 'Partial Plate Armor^ [%P6,%P4]';
					tval: 35; tchar: '['; flags: $00000000; p1: 0; cost: 900;
					subval: 9; weight: 260; number: 1; tohit: -3; todam: 0;
					ac: 22; toac: 0; damage: '1d6'; level: 42),
			(name: 'Metal Lamellar Armor^ [%P6,%P4] ';
					tval: 35; tchar: '['; flags: $00000000; p1: 0; cost: 950;
					subval: 10; weight: 340; number: 1; tohit: -3; todam: 0;
					ac: 23; toac: 0; damage: '1d6'; level: 44),
			(name: 'Full Plate Armor^ [%P6,%P4]';
					tval: 35; tchar: '['; flags: $00000000; p1: 0; cost: 1050;
					subval: 11; weight: 380; number: 1; tohit: -3; todam: 0;
					ac: 25; toac: 0; damage: '2d4'; level: 48),
			(name: 'Ribbed Plate Armor^ [%P6,%P4]';
					tval: 35; tchar: '['; flags: $00000000; p1: 0; cost: 1200;
					subval: 12; weight: 380; number: 1; tohit: -3; todam: 0;
					ac: 28; toac: 0; damage: '2d4'; level: 50),
			(name: '& Cloak^ [%P6,%P4] ';
					tval: 32; tchar: '('; flags: $00000000; p1: 0; cost: 3;
					subval: 1; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 1; toac: 0; damage: '0d0'; level: 1),
			(name: '& Set of Leather Gloves^ [%P6,%P4] ';
					tval: 31; tchar: ']'; flags: $00000000; p1: 0; cost: 3;
					subval: 1; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 1; toac: 0; damage: '0d0'; level: 1),
			(name: '& Set of Gauntlets^ [%P6,%P4]';
					tval: 31; tchar: ']'; flags: $00000000; p1: 0; cost: 35;
					subval: 2; weight: 25; number: 1; tohit: 0; todam: 0;
					ac: 2; toac: 0; damage: '1d1'; level: 12),
			(name: '& Small Leather Shield^ [%P6,%P4]';
					tval: 34; tchar: ')'; flags: $00000000; p1: 0; cost: 30;
					subval: 1; weight: 50; number: 1; tohit: 0; todam: 0;
					ac: 2; toac: 0; damage: '1d1'; level: 3),
			(name: '& Medium Leather Shield^ [%P6,%P4] ';
					tval: 34; tchar: ')'; flags: $00000000; p1: 0; cost: 60;
					subval: 2; weight: 75; number: 1; tohit: 0; todam: 0;
					ac: 3; toac: 0; damage: '1d2'; level: 8),
			(name: '& Large Leather Shield^ [%P6,%P4]';
					tval: 34; tchar: ')'; flags: $00000000; p1: 0; cost: 120;
					subval: 3; weight: 100; number: 1; tohit: 0; todam: 0;
					ac: 4; toac: 0; damage: '1d2'; level: 15),
			(name: '& Small Metal Shield^ [%P6,%P4] ';
					tval: 34; tchar: ')'; flags: $00000000; p1: 0; cost: 50;
					subval: 4; weight: 65; number: 1; tohit: 0; todam: 0;
					ac: 3; toac: 0; damage: '1d2'; level: 10),
			(name: '& Medium Metal Shield^ [%P6,%P4]';
					tval: 34; tchar: ')'; flags: $00000000; p1: 0; cost: 125;
					subval: 5; weight: 90; number: 1; tohit: 0; todam: 0;
					ac: 4; toac: 0; damage: '1d3'; level: 20),
			(name: '& Large Metal Shield^ [%P6,%P4] ';
					tval: 34; tchar: ')'; flags: $00000000; p1: 0; cost: 200;
					subval: 6; weight: 120; number: 1; tohit: 0; todam: 0;
					ac: 5; toac: 0; damage: '1d3'; level: 30),
			(name: '& %R Ring| of Gain Strength^ (%P1) ';
					tval: 45; tchar: '='; flags: $00000001; p1: 0; cost: 400;
					subval: 1; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 30),
			(name: '& %R Ring| of Gain Dexterity^ (%P1)';
					tval: 45; tchar: '='; flags: $00000002; p1: 0; cost: 400;
					subval: 2; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 30),
			(name: '& %R Ring| of Gain Constitution^ (%P1)';
					tval: 45; tchar: '='; flags: $00000004; p1: 0; cost: 400;
					subval: 3; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 30),
			(name: '& %R Ring| of Gain Intelligence^ (%P1)';
					tval: 45; tchar: '='; flags: $00000008; p1: 0; cost: 350;
					subval: 4; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 30),
			(name: '& %R Ring| of Speed^ (%P1) ';
					tval: 45; tchar: '='; flags: $00001000; p1: 0; cost: 3000;
					subval: 7; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50),
			(name: '& %R Ring| of Searching^ (%P1)';
					tval: 45; tchar: '='; flags: $00000040; p1: 0; cost: 250;
					subval: 8; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 7),
			(name: '& %R Ring| of Telportation^';
					tval: 45; tchar: '='; flags: $80000400; p1: 0; cost: 0;
					subval: 9; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 7),
			(name: '& %R Ring| of Slow Digestion^';
					tval: 45; tchar: '='; flags: $00000080; p1: 0; cost: 250;
					subval: 10; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 7),
			(name: '& %R Ring| of Resist Fire^';
					tval: 45; tchar: '='; flags: $00080000; p1: 0; cost: 250;
					subval: 11; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 14),
			(name: '& %R Ring| of Resist Cold^';
					tval: 45; tchar: '='; flags: $00200000; p1: 0; cost: 250;
					subval: 12; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 14),
			(name: '& %R Ring| of Feather Falling^';
					tval: 45; tchar: '='; flags: $04000000; p1: 0; cost: 200;
					subval: 13; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 7),
			(name: '& %R Ring| of Adornment^';
					tval: 45; tchar: '='; flags: $00000000; p1: 0; cost: 20;
					subval: 14; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 7),
			(name: '& %R Ring| of Adornment^';
					tval: 45; tchar: '='; flags: $00000000; p1: 0; cost: 30;
					subval: 15; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 7),
			(name: '& %R Ring| of Weakness^';
					tval: 45; tchar: '='; flags: $00000001; p1: -5; cost: 0;
					subval: 16; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 7),
			(name: '& %R Ring| of Lordly Protection (FIRE)^';
					tval: 45; tchar: '='; flags: $00080000; p1: 0; cost: 1200;
					subval: 17; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 5; damage: '0d0'; level: 50),
			(name: '& %R Ring| of Lordly Protection (ACID)^';
					tval: 45; tchar: '='; flags: $00100000; p1: 0; cost: 1200;
					subval: 18; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 5; damage: '0d0'; level: 50),
			(name: '& %R Ring| of Lordly Protection (COLD)^';
					tval: 45; tchar: '='; flags: $00200000; p1: 0; cost: 1200;
					subval: 19; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 5; damage: '0d0'; level: 50),
			(name: '& %R Ring| of WOE^';
					tval: 45; tchar: '='; flags: $80000640; p1: -5; cost: 0;
					subval: 20; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: -3; damage: '0d0'; level: 50),
			(name: '& %R Ring| of Stupidity^';
					tval: 45; tchar: '='; flags: $80000008; p1: -5; cost: 0;
					subval: 21; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 20),
			(name: '& %R Ring| of Increase Damage^ (%P3)';
					tval: 45; tchar: '='; flags: $00000000; p1: 0; cost: 100;
					subval: 22; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 20),
			(name: '& %R Ring| of Increase To-Hit^ (%P2)';
					tval: 45; tchar: '='; flags: $00000000; p1: 0; cost: 100;
					subval: 23; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 20),
			(name: '& %R Ring| of Protection^ [%P4] ';
					tval: 45; tchar: '='; flags: $00000000; p1: 0; cost: 100;
					subval: 24; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 7),
			(name: '& %R Ring| of Aggravate Monster^';
					tval: 45; tchar: '='; flags: $80000200; p1: 0; cost: 0;
					subval: 25; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 7),
			(name: '& %R Ring| of See Invisible^';
					tval: 45; tchar: '='; flags: $01000000; p1: 0; cost: 340;
					subval: 26; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 40),
			(name: '& %R Ring| of Sustain Strength^';
					tval: 45; tchar: '='; flags: $00400000; p1: 1; cost: 750;
					subval: 27; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 44),
			(name: '& %R Ring| of Sustain Intelligence^';
					tval: 45; tchar: '='; flags: $00400000; p1: 2; cost: 600;
					subval: 28; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 44),
			(name: '& %R Ring| of Sustain Wisdom^';
					tval: 45; tchar: '='; flags: $00400000; p1: 3; cost: 600;
					subval: 29; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 44),
			(name: '& %R Ring| of Sustain Constitution^';
					tval: 45; tchar: '='; flags: $00400000; p1: 4; cost: 750;
					subval: 30; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 44),
			(name: '& %R Ring| of Sustain Dexterity^';
					tval: 45; tchar: '='; flags: $00400000; p1: 5; cost: 750;
					subval: 31; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 44),
			(name: '& %R Ring| of Sustain Charisma^ ';
					tval: 45; tchar: '='; flags: $00400000; p1: 6; cost: 500;
					subval: 32; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 7),
			(name: '& %R Ring| of Slaying^';
					tval: 45; tchar: '='; flags: $00000000; p1: 6; cost: 1000;
					subval: 33; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50),
			(name: '& %A Amulet| of Adornment^ ';
					tval: 40; tchar: '"'; flags: $00000000; p1: 0; cost: 20;
					subval: 11; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 16),
			(name: '& %A Amulet| of Adornment^ ';
					tval: 40; tchar: '"'; flags: $00000000; p1: 0; cost: 30;
					subval: 12; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 16),
			(name: '& %A Amulet| of Wisdom^ (%P1)';
					tval: 40; tchar: '"'; flags: $00000010; p1: 0; cost: 300;
					subval: 5; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 20),
			(name: '& %A Amulet| of Charisma^ (%P1) ';
					tval: 40; tchar: '"'; flags: $00000020; p1: 0; cost: 250;
					subval: 6; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 20),
			(name: '& %A Amulet| of Searching^ (%P1)';
					tval: 40; tchar: '"'; flags: $00000040; p1: 0; cost: 250;
					subval: 7; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 14),
			(name: '& %A Amulet| of Telportation^';
					tval: 40; tchar: '"'; flags: $80000400; p1: 0; cost: 0;
					subval: 8; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 14),
			(name: '& %A Amulet| of Slow Digestion^ ';
					tval: 40; tchar: '"'; flags: $00000080; p1: 0; cost: 200;
					subval: 9; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 14),
			(name: '& %A Amulet| of Resist Acid^ ';
					tval: 40; tchar: '"'; flags: $00100000; p1: 0; cost: 300;
					subval: 10; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 24),
			(name: '& %A Amulet| of the Magi^';
					tval: 40; tchar: '"'; flags: $01800040; p1: 0; cost: 5000;
					subval: 13; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 3; damage: '0d0'; level: 50),
			(name: '& %A Amulet| of DOOM^';
					tval: 40; tchar: '"'; flags: $8000007F; p1: -5; cost: 0;
					subval: 14; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50),
			(name: '& Scroll~ %T| of Enchant Weapon To-Hit';
					tval: 70; tchar: '?'; flags: $00000001; p1: 0; cost: 125;
					subval: 257; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 12),
			(name: '& Scroll~ %T| of Enchant Weapon To-Dam';
					tval: 70; tchar: '?'; flags: $00000002; p1: 0; cost: 125;
					subval: 258; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 12),
			(name: '& Scroll~ %T| of Enchant Armor';
					tval: 70; tchar: '?'; flags: $00000004; p1: 0; cost: 125;
					subval: 259; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 12),
			(name: '& Scroll~ %T| of Identify I';
					tval: 70; tchar: '?'; flags: $00000008; p1: 0; cost: 50;
					subval: 260; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 1),
			(name: '& Scroll~ %T| of Identify II';
					tval: 70; tchar: '?'; flags: $00000008; p1: 0; cost: 50;
					subval: 260; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 5),
			(name: '& Scroll~ %T| of Identify III';
					tval: 70; tchar: '?'; flags: $00000008; p1: 0; cost: 50;
					subval: 260; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 10),
			(name: '& Scroll~ %T| of Remove Curse';
					tval: 70; tchar: '?'; flags: $00000010; p1: 0; cost: 100;
					subval: 261; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 7),
			(name: '& Scroll~ %T| of Light I';
					tval: 70; tchar: '?'; flags: $00000020; p1: 0; cost: 15;
					subval: 262; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),
			(name: '& Scroll~ %T| of Light II';
					tval: 70; tchar: '?'; flags: $00000020; p1: 0; cost: 15;
					subval: 262; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 3),
			(name: '& Scroll~ %T| of Light III';
					tval: 70; tchar: '?'; flags: $00000020; p1: 0; cost: 15;
					subval: 262; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 7),
			(name: '& Scroll~ %T| of Summon Monster ';
					tval: 70; tchar: '?'; flags: $00000040; p1: 0; cost: 0;
					subval: 263; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 1),
			(name: '& Scroll~ %T| of Phase Door';
					tval: 70; tchar: '?'; flags: $00000080; p1: 0; cost: 15;
					subval: 264; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 1),
			(name: '& Scroll~ %T| of Teleport';
					tval: 70; tchar: '?'; flags: $00000100; p1: 0; cost: 40;
					subval: 265; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 10),
			(name: '& Scroll~ %T| of Monster Confusion ';
					tval: 70; tchar: '?'; flags: $00000400; p1: 0; cost: 30;
					subval: 267; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 5),
			(name: '& Scroll~ %T| of Magic Mapping';
					tval: 70; tchar: '?'; flags: $00000800; p1: 0; cost: 40;
					subval: 268; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 5),
			(name: '& Scroll~ %T| of Sleep Monster';
					tval: 70; tchar: '?'; flags: $00001000; p1: 0; cost: 35;
					subval: 269; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 5),
			(name: '& Scroll~ %T| of Rune of Protection';
					tval: 70; tchar: '?'; flags: $00002000; p1: 0; cost: 500;
					subval: 270; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50),
			(name: '& Scroll~ %T| of Treasure Detection';
					tval: 70; tchar: '?'; flags: $00004000; p1: 0; cost: 15;
					subval: 271; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),
			(name: '& Scroll~ %T| of Object Detection';
					tval: 70; tchar: '?'; flags: $00008000; p1: 0; cost: 15;
					subval: 272; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),
			(name: '& Scroll~ %T| of Trap Detection I ';
					tval: 70; tchar: '?'; flags: $00010000; p1: 0; cost: 35;
					subval: 273; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 5),
			(name: '& Scroll~ %T| of Trap Detection II ';
					tval: 70; tchar: '?'; flags: $00010000; p1: 0; cost: 35;
					subval: 273; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 8),
			(name: '& Scroll~ %T| of Trap Detection II ';
					tval: 70; tchar: '?'; flags: $00010000; p1: 0; cost: 35;
					subval: 273; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 12),
			(name: '& Scroll~ %T| of Door/Stair Location I ';
					tval: 70; tchar: '?'; flags: $00020000; p1: 0; cost: 35;
					subval: 274; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 5),
			(name: '& Scroll~ %T| of Door/Stair Location II ';
					tval: 70; tchar: '?'; flags: $00020000; p1: 0; cost: 35;
					subval: 274; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 10),
			(name: '& Scroll~ %T| of Door/Stair Location III ';
					tval: 70; tchar: '?'; flags: $00020000; p1: 0; cost: 35;
					subval: 274; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 15),
			(name: '& Scroll~ %T| of Mass Genocide';
					tval: 70; tchar: '?'; flags: $00040000; p1: 0; cost: 1000;
					subval: 275; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50),
			(name: '& Scroll~ %T| of Detect Invisible';
					tval: 70; tchar: '?'; flags: $00080000; p1: 0; cost: 15;
					subval: 276; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 1),
			(name: '& Scroll~ %T| of Aggravate Monster ';
					tval: 70; tchar: '?'; flags: $00100000; p1: 0; cost: 0;
					subval: 277; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 5),
			(name: '& Scroll~ %T| of Trap Creation';
					tval: 70; tchar: '?'; flags: $00200000; p1: 0; cost: 0;
					subval: 278; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 12),
			(name: '& Scroll~ %T| of Trap/Door Destruction';
					tval: 70; tchar: '?'; flags: $00400000; p1: 0; cost: 50;
					subval: 279; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 12),
			(name: '& Scroll~ %T| of Door Creation';
					tval: 70; tchar: '?'; flags: $00800000; p1: 0; cost: 100;
					subval: 280; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 12),
			(name: '& Scroll~ %T| of Recharging';
					tval: 70; tchar: '?'; flags: $01000000; p1: 0; cost: 200;
					subval: 281; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 40),
			(name: '& Scroll~ %T| of Genocide';
					tval: 70; tchar: '?'; flags: $02000000; p1: 0; cost: 750;
					subval: 282; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 35),
			(name: '& Scroll~ %T| of Darkness';
					tval: 70; tchar: '?'; flags: $04000000; p1: 0; cost: 0;
					subval: 283; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 1),
			(name: '& Scroll~ %T| of Protection from Evil';
					tval: 70; tchar: '?'; flags: $08000000; p1: 0; cost: 50;
					subval: 284; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 30),
			(name: '& Scroll~ %T| of Create Food ';
					tval: 70; tchar: '?'; flags: $10000000; p1: 0; cost: 10;
					subval: 285; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 5),
			(name: '& Scroll~ %T| of Dispell Undead ';
					tval: 70; tchar: '?'; flags: $20000000; p1: 0; cost: 200;
					subval: 286; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 40),
			(name: '& Scroll~ %T| of *Enchant Weapon*';
					tval: 71; tchar: '?'; flags: $00000001; p1: 0; cost: 500;
					subval: 257; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50),
			(name: '& Scroll~ %T| of Curse Weapon';
					tval: 71; tchar: '?'; flags: $00000002; p1: 0; cost: 0;
					subval: 258; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50),
			(name: '& Scroll~ %T| of *Enchant Armor*';
					tval: 71; tchar: '?'; flags: $00000004; p1: 0; cost: 500;
					subval: 259; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50),
			(name: '& Scroll~ %T| of Curse Armor ';
					tval: 71; tchar: '?'; flags: $00000008; p1: 0; cost: 0;
					subval: 260; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50),
			(name: '& Scroll~ %T| of Summon Undead';
					tval: 71; tchar: '?'; flags: $00000010; p1: 0; cost: 0;
					subval: 261; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 15),
			(name: '& Scroll~ %T| of Blessing';
					tval: 71; tchar: '?'; flags: $00000020; p1: 0; cost: 15;
					subval: 262; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 1),
			(name: '& Scroll~ %T| of Holy Chant';
					tval: 71; tchar: '?'; flags: $00000040; p1: 0; cost: 40;
					subval: 263; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 12),
			(name: '& Scroll~ %T| of Holy Prayer ';
					tval: 71; tchar: '?'; flags: $00000080; p1: 0; cost: 80;
					subval: 264; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 24),
			(name: '& Scroll~ %T| of Word-of-Recall ';
					tval: 71; tchar: '?'; flags: $00000100; p1: 0; cost: 150;
					subval: 265; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 5),
			(name: '& Scroll~ %T| of *Destruction*';
					tval: 71; tchar: '?'; flags: $00000200; p1: 0; cost: 250;
					subval: 266; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 40),
			(name: '& %C Potion~| of Gain Strength I';
					tval: 75; tchar: '!'; flags: $00000001; p1: 0; cost: 200;
					subval: 257; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 10),
			(name: '& %C Potion~| of Gain Strength II';
					tval: 75; tchar: '!'; flags: $00000001; p1: 0; cost: 200;
					subval: 257; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 25),
			(name: '& %C Potion~| of Poison ';
					tval: 75; tchar: '!'; flags: $00000002; p1: 0; cost: 0;
					subval: 258; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 3),
			(name: '& %C Potion~| of Restore Strength';
					tval: 75; tchar: '!'; flags: $00000004; p1: 0; cost: 300;
					subval: 259; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 20),
			(name: '& %C Potion~| of Gain Intelligence I ';
					tval: 75; tchar: '!'; flags: $00000008; p1: 0; cost: 300;
					subval: 260; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 10),
			(name: '& %C Potion~| of Gain Intelligence II ';
					tval: 75; tchar: '!'; flags: $00000008; p1: 0; cost: 300;
					subval: 260; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 25),
			(name: '& %C Potion~| of Lose Intelligence ';
					tval: 75; tchar: '!'; flags: $00000010; p1: 0; cost: 0;
					subval: 261; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 25),
			(name: '& %C Potion~| of Restore Intelligence';
					tval: 75; tchar: '!'; flags: $00000020; p1: 0; cost: 300;
					subval: 262; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 20),
			(name: '& %C Potion~| of Gain Wisdom I ';
					tval: 75; tchar: '!'; flags: $00000040; p1: 0; cost: 300;
					subval: 263; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 10),
			(name: '& %C Potion~| of Gain Wisdom II ';
					tval: 75; tchar: '!'; flags: $00000040; p1: 0; cost: 300;
					subval: 263; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 25),
			(name: '& %C Potion~| of Lose Wisdom ';
					tval: 75; tchar: '!'; flags: $00000080; p1: 0; cost: 0;
					subval: 264; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 25),
			(name: '& %C Potion~| of Restore Wisdom ';
					tval: 75; tchar: '!'; flags: $00000100; p1: 0; cost: 300;
					subval: 265; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 20),
			(name: '& %C Potion~| of Charisma I';
					tval: 75; tchar: '!'; flags: $00000200; p1: 0; cost: 300;
					subval: 266; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 10),
			(name: '& %C Potion~| of Charisma II';
					tval: 75; tchar: '!'; flags: $00000200; p1: 0; cost: 300;
					subval: 266; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 25),
			(name: '& %C Potion~| of Ugliness';
					tval: 75; tchar: '!'; flags: $00000400; p1: 0; cost: 0;
					subval: 267; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 25),
			(name: '& %C Potion~| of Restore Charisma';
					tval: 75; tchar: '!'; flags: $00000800; p1: 0; cost: 300;
					subval: 268; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 20),
			(name: '& %C Potion~| of Cure Light Wounds I ';
					tval: 75; tchar: '!'; flags: $10001000; p1: 50; cost: 15;
					subval: 269; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),
			(name: '& %C Potion~| of Cure Light Wounds II ';
					tval: 75; tchar: '!'; flags: $10001000; p1: 50; cost: 15;
					subval: 269; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 1),
			(name: '& %C Potion~| of Cure Light Wounds III ';
					tval: 75; tchar: '!'; flags: $10001000; p1: 50; cost: 15;
					subval: 269; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 2),
			(name: '& %C Potion~| of Cure Serious Wounds ';
					tval: 75; tchar: '!'; flags: $30002000; p1: 100; cost: 40;
					subval: 270; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 3),
			(name: '& %C Potion~| of Cure Critical Wounds';
					tval: 75; tchar: '!'; flags: $70004000; p1: 100; cost: 100;
					subval: 271; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 5),
			(name: '& %C Potion~| of Healing';
					tval: 75; tchar: '!'; flags: $70008000; p1: 200; cost: 200;
					subval: 272; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 12),
			(name: '& %C Potion~| of Gain Constitution I ';
					tval: 75; tchar: '!'; flags: $00010000; p1: 0; cost: 300;
					subval: 273; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 10),
			(name: '& %C Potion~| of Gain Constitution II ';
					tval: 75; tchar: '!'; flags: $00010000; p1: 0; cost: 300;
					subval: 273; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 25),
			(name: '& %C Potion~| of Gain Experience';
					tval: 75; tchar: '!'; flags: $00020000; p1: 0; cost: 2500;
					subval: 274; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50),
			(name: '& %C Potion~| of Sleep';
					tval: 75; tchar: '!'; flags: $10040000; p1: 100; cost: 0;
					subval: 275; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),
			(name: '& %C Potion~| of Blindness ';
					tval: 75; tchar: '!'; flags: $00080000; p1: 0; cost: 0;
					subval: 276; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),
			(name: '& %C Potion~| of Confusion ';
					tval: 75; tchar: '!'; flags: $00100000; p1: 50; cost: 0;
					subval: 277; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),
			(name: '& %C Potion~| of Poison ';
					tval: 75; tchar: '!'; flags: $00200000; p1: 0; cost: 0;
					subval: 278; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 3),
			(name: '& %C Potion~| of Haste Self';
					tval: 75; tchar: '!'; flags: $00400000; p1: 0; cost: 75;
					subval: 279; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 1),
			(name: '& %C Potion~| of Slowness';
					tval: 75; tchar: '!'; flags: $00800000; p1: 50; cost: 0;
					subval: 280; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 1),
			(name: '& Icky Green Potion~| of Slime Mold Juice';
					tval: 75; tchar: '!'; flags: $30000000; p1: 400; cost: 2;
					subval: 281; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),
			(name: '& Light Brown Potion~| of Apple Juice';
					tval: 75; tchar: '!'; flags: $00000000; p1: 250; cost: 1;
					subval: 282; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),
			(name: '& Clear Potion~| of Water';
					tval: 75; tchar: '!'; flags: $00000000; p1: 200; cost: 0;
					subval: 283; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),
			(name: '& %C Potion~| of Gain Dexterity I ';
					tval: 75; tchar: '!'; flags: $02000000; p1: 0; cost: 300;
					subval: 284; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 10),
			(name: '& %C Potion~| of Gain Dexterity II ';
					tval: 75; tchar: '!'; flags: $02000000; p1: 0; cost: 300;
					subval: 284; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 25),
			(name: '& %C Potion~| of Restore Dexterity ';
					tval: 75; tchar: '!'; flags: $04000000; p1: 0; cost: 300;
					subval: 285; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 20),
			(name: '& %C Potion~| of Restore Constitution';
					tval: 75; tchar: '!'; flags: $68000000; p1: 0; cost: 300;
					subval: 286; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 20),
			(name: '& %C Potion~| of Learning I';
					tval: 76; tchar: '!'; flags: $00000001; p1: 0; cost: 200;
					subval: 287; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 10),
			(name: '& %C Potion~| of Learning II';
					tval: 76; tchar: '!'; flags: $00000001; p1: 0; cost: 200;
					subval: 287; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 25),
			(name: '& %C Potion~| of Lose Memories';
					tval: 76; tchar: '!'; flags: $00000002; p1: 0; cost: 0;
					subval: 288; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 10),
			(name: '& %C Potion~| of Salt Water';
					tval: 76; tchar: '!'; flags: $00000004; p1: 0; cost: 0;
					subval: 289; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),
			(name: '& %C Potion~| of Invulnerability';
					tval: 76; tchar: '!'; flags: $00000008; p1: 0; cost: 250;
					subval: 290; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 40),
			(name: '& %C Potion~| of Heroism';
					tval: 76; tchar: '!'; flags: $00000010; p1: 0; cost: 35;
					subval: 291; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 1),
			(name: '& %C Potion~| of Super Heroism';
					tval: 76; tchar: '!'; flags: $00000020; p1: 0; cost: 100;
					subval: 292; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 3),
			(name: '& %C Potion~| of Boldliness';
					tval: 76; tchar: '!'; flags: $00000040; p1: 0; cost: 10;
					subval: 293; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 1),
			(name: '& %C Potion~| of Restore Life Levels ';
					tval: 76; tchar: '!'; flags: $00000080; p1: 0; cost: 400;
					subval: 294; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 25),
			(name: '& %C Potion~| of Resist Heat ';
					tval: 76; tchar: '!'; flags: $00000100; p1: 0; cost: 30;
					subval: 295; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 1),
			(name: '& %C Potion~| of Resist Cold ';
					tval: 76; tchar: '!'; flags: $00000200; p1: 0; cost: 30;
					subval: 296; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 1),
			(name: '& %C Potion~| of Detect Invisible';
					tval: 76; tchar: '!'; flags: $00000400; p1: 0; cost: 50;
					subval: 297; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 3),
			(name: '& %C Potion~| of Slow Poison ';
					tval: 76; tchar: '!'; flags: $00000800; p1: 0; cost: 25;
					subval: 298; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 1),
			(name: '& %C Potion~| of Neutralize Poison ';
					tval: 76; tchar: '!'; flags: $00001000; p1: 0; cost: 75;
					subval: 299; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 5),
			(name: '& %C Potion~| of Restore Mana';
					tval: 76; tchar: '!'; flags: $00002000; p1: 0; cost: 35;
					subval: 300; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 25),
			(name: '& %C Potion~| Infra-Vision ';
					tval: 76; tchar: '!'; flags: $00004000; p1: 0; cost: 20;
					subval: 301; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 3),
			(name: '& %C Potion~| of Longevity ';
					tval: 76; tchar: '!'; flags: $00008000; p1: 0; cost: 50;
					subval: 302; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 55),
			(name: '& %C Potion~| of Gain MANA ';
					tval: 76; tchar: '!'; flags: $00010000; p1: 0; cost: 50;
					subval: 303; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 55),
			(name: '& Flask~ of oil ';
					tval: 77; tchar: '!'; flags: $00040000; p1: 7500; cost: 3;
					subval: 257; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d6'; level: 1),
			(name: '& %M Wand| of Light^ (%P1 charges) ';
					tval: 65; tchar: '-'; flags: $00000001; p1: 0; cost: 200;
					subval: 1; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 2),
			(name: '& %M Wand| of Lightning Bolts^ (%P1 charges) ';
					tval: 65; tchar: '-'; flags: $00000002; p1: 0; cost: 600;
					subval: 2; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 15),
			(name: '& %M Wand| of Frost Bolts^ (%P1 charges)';
					tval: 65; tchar: '-'; flags: $00000004; p1: 0; cost: 800;
					subval: 3; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 20),
			(name: '& %M Wand| of Fire Bolts^ (%P1 charges) ';
					tval: 65; tchar: '-'; flags: $00000008; p1: 0; cost: 1000;
					subval: 4; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 30),
			(name: '& %M Wand| of Stone-to-Mud^ (%P1 charges)';
					tval: 65; tchar: '-'; flags: $00000010; p1: 0; cost: 300;
					subval: 5; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 12),
			(name: '& %M Wand| of Polymorph^ (%P1 charges)';
					tval: 65; tchar: '-'; flags: $00000020; p1: 0; cost: 400;
					subval: 6; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 20),
			(name: '& %M Wand| of Heal Monster^ (%P1 charges)';
					tval: 65; tchar: '-'; flags: $00000040; p1: 0; cost: 0;
					subval: 7; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 2),
			(name: '& %M Wand| of Haste Monster^ (%P1 charges) ';
					tval: 65; tchar: '-'; flags: $00000080; p1: 0; cost: 0;
					subval: 8; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 2),
			(name: '& %M Wand| of Slow Monster^ (%P1 charges)';
					tval: 65; tchar: '-'; flags: $00000100; p1: 0; cost: 500;
					subval: 9; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 2),
			(name: '& %M Wand| of Confuse Monster^ (%P1 charges) ';
					tval: 65; tchar: '-'; flags: $00000200; p1: 0; cost: 400;
					subval: 10; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 2),
			(name: '& %M Wand| of Sleep Monster^ (%P1 charges) ';
					tval: 65; tchar: '-'; flags: $00000400; p1: 0; cost: 500;
					subval: 11; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 7),
			(name: '& %M Wand| of Drain Life^ (%P1 charges) ';
					tval: 65; tchar: '-'; flags: $00000800; p1: 0; cost: 1200;
					subval: 12; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50),
			(name: '& %M Wand| of Trap/Door destruction^ (%P1 charges)';
					tval: 65; tchar: '-'; flags: $00001000; p1: 0; cost: 100;
					subval: 13; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 12),
			(name: '& %M Wand| of Magic Missle^ (%P1 charges)';
					tval: 65; tchar: '-'; flags: $00002000; p1: 0; cost: 200;
					subval: 14; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 2),
			(name: '& %M Wand| of Wall Building^ (%P1 charges) ';
					tval: 65; tchar: '-'; flags: $00004000; p1: 0; cost: 400;
					subval: 15; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 25),
			(name: '& %M Wand| of Clone Monster^ (%P1 charges) ';
					tval: 65; tchar: '-'; flags: $00008000; p1: 0; cost: 0;
					subval: 16; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 2),
			(name: '& %M Wand| of Teleport Away^ (%P1 charges) ';
					tval: 65; tchar: '-'; flags: $00010000; p1: 0; cost: 350;
					subval: 17; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 20),
			(name: '& %M Wand| of Disarming^ (%P1 charges)';
					tval: 65; tchar: '-'; flags: $00020000; p1: 0; cost: 700;
					subval: 18; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 20),
			(name: '& %M Wand| of Lightning Balls^ (%P1 charges) ';
					tval: 65; tchar: '-'; flags: $00040000; p1: 0; cost: 1200;
					subval: 19; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 35),
			(name: '& %M Wand| of Cold Balls^ (%P1 charges) ';
					tval: 65; tchar: '-'; flags: $00080000; p1: 0; cost: 1500;
					subval: 20; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 40),
			(name: '& %M Wand| of Fire Balls^ (%P1 charges) ';
					tval: 65; tchar: '-'; flags: $00100000; p1: 0; cost: 1800;
					subval: 21; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50),
			(name: '& %M Wand| of Stinking Cloud^ (%P1 charges)';
					tval: 65; tchar: '-'; flags: $00200000; p1: 0; cost: 400;
					subval: 21; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 5),
			(name: '& %M Wand| of Acid Balls^ (%P1 charges) ';
					tval: 65; tchar: '-'; flags: $00400000; p1: 0; cost: 1650;
					subval: 21; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 48),
			(name: '& %M Wand| of Wonder^ (%P1 charges)';
					tval: 65; tchar: '-'; flags: $00800000; p1: 0; cost: 250;
					subval: 22; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 2),
			(name: '& %W Staff| of Light^ (%P1 charges)';
					tval: 55; tchar: '_'; flags: $00000001; p1: 0; cost:
					250; subval: 1; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 5),
			(name: '& %W Staff| of Door/Stair Location^ (%P1 charges)';
					tval: 55; tchar: '_'; flags: $00000002; p1: 0; cost:
					350; subval: 2; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 10),
			(name: '& %W Staff| of Trap Location^ (%P1 charges)';
					tval: 55; tchar: '_'; flags: $00000004; p1: 0; cost:
					350; subval: 3; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 10),
			(name: '& %W Staff| of Treasure Location^ (%P1 charges) ';
					tval: 55; tchar: '_'; flags: $00000008; p1: 0; cost:
					200; subval: 4; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 5),
			(name: '& %W Staff| of Object Location^ (%P1 charges)';
					tval: 55; tchar: '_'; flags: $00000010; p1: 0; cost:
					200; subval: 5; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 5),
			(name: '& %W Staff| of Earthquakes^ (%P1 charges)';
					tval: 55; tchar: '_'; flags: $00000040; p1: 0; cost:
					350; subval: 7; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 40),
			(name: '& %W Staff| of Summoning^ (%P1 charges) ';
					tval: 55; tchar: '_'; flags: $00000080; p1: 0; cost:
					0; subval: 8; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 10),
			(name: '& %W Staff| of Summoning^ (%P1 charges) ';
					tval: 55; tchar: '_'; flags: $00000080; p1: 0; cost:
					0; subval: 8; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50),
			(name: '& %W Staff| of *Destruction*^ (%P1 charges)';
					tval: 55; tchar: '_'; flags: $00000200; p1: 0; cost:
					2500; subval: 10; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50),
			(name: '& %W Staff| of Starlite^ (%P1 charges)';
					tval: 55; tchar: '_'; flags: $00000400; p1: 0; cost:
					800; subval: 11; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 20),
			(name: '& %W Staff| of Haste Monsters^ (%P1 charges) ';
					tval: 55; tchar: '_'; flags: $00000800; p1: 0; cost:
					0; subval: 12; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 10),
			(name: '& %W Staff| of Slow Monsters^ (%P1 charges)';
					tval: 55; tchar: '_'; flags: $00001000; p1: 0; cost:
					800; subval: 13; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 10),
			(name: '& %W Staff| of Sleep Monsters^ (%P1 charges) ';
					tval: 55; tchar: '_'; flags: $00002000; p1: 0; cost:
					700; subval: 14; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 10),
			(name: '& %W Staff| of Cure Light Wounds^ (%P1 charges) ';
					tval: 55; tchar: '_'; flags: $00004000; p1: 0; cost:
					350; subval: 15; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 5),
			(name: '& %W Staff| of Detect Invisible^ (%P1 charges)';
					tval: 55; tchar: '_'; flags: $00008000; p1: 0; cost:
					200; subval: 16; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 5),
			(name: '& %W Staff| of Speed^ (%P1 charges)';
					tval: 55; tchar: '_'; flags: $00010000; p1: 0; cost:
					800; subval: 17; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 40),
			(name: '& %W Staff| of Slowness^ (%P1 charges)';
					tval: 55; tchar: '_'; flags: $00020000; p1: 0; cost:
					0; subval: 18; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 40),
			(name: '& %W Staff| of Mass Polymorph^ (%P1 charges) ';
					tval: 55; tchar: '_'; flags: $00040000; p1: 0; cost:
					750; subval: 19; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 46),
			(name: '& %W Staff| of Remove Curse^ (%P1 charges) ';
					tval: 55; tchar: '_'; flags: $00080000; p1: 0; cost:
					500; subval: 20; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 47),
			(name: '& %W Staff| of Detect Evil^ (%P1 charges)';
					tval: 55; tchar: '_'; flags: $00100000; p1: 0; cost:
					350; subval: 21; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 20),
			(name: '& %W Staff| of Curing^ (%P1 charges) ';
					tval: 55; tchar: '_'; flags: $00200000; p1: 0; cost:
					1000; subval: 22; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 25),
			(name: '& %W Staff| of Dispell Evil^ (%P1 charges) ';
					tval: 55; tchar: '_'; flags: $00400000; p1: 0; cost:
					1200; subval: 23; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 49),
			(name: '& %W Staff| of Darkness^ (%P1 charges)';
					tval: 55; tchar: '_'; flags: $01000000; p1: 0; cost:
					0; subval: 25; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50),
			(name: '& %W Staff| of Darkness^ (%P1 charges)';
					tval: 55; tchar: '_'; flags: $01000000; p1: 0; cost:
					0; subval: 25; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 5),
			(name: '& Book of Magic Spells [Beginners-Magik]';
					tval: 90; tchar: '?'; flags: $0000007F; p1: 0; cost: 25;
					subval: 257; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 40),
			(name: '& Book of Magic Spells [Magik I]';
					tval: 90; tchar: '?'; flags: $0000FF80; p1: 0; cost: 100;
					subval: 258; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 40),
			(name: '& Book of Magic Spells [Magik II]';
					tval: 90; tchar: '?'; flags: $00FF0000; p1: 0; cost: 400;
					subval: 259; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 40),
			(name: '& Book of Magic Spells [The Mages Guide to Power]';
					tval: 90; tchar: '?'; flags: $7F000000; p1: 0; cost: 800;
					subval: 261; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 40),
			(name: '& Holy Book of Prayers [Beginners Handbook]';
					tval: 91; tchar: '?'; flags: $000000FF; p1: 0; cost: 25;
					subval: 258; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 40),
			(name: '& Holy Book of Prayers [Words of Wisdom]';
					tval: 91; tchar: '?'; flags: $0000FF00; p1: 0; cost: 100;
					subval: 259; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 40),
			(name: '& Holy Book of Prayers [Chants and Blessings]';
					tval: 91; tchar: '?'; flags: $01FF0000; p1: 0; cost: 300;
					subval: 260; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 40),
			(name: '& Holy Book of Prayers [Exorcism and Dispelling]';
					tval: 91; tchar: '?'; flags: $7E000000; p1: 0; cost: 900;
					subval: 261; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 40),
			(name: '& Small wooden chest ';
					tval: 2; tchar: '&'; flags: $0F000000; p1: 0; cost: 20;
					subval: 1; weight: 250; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d3'; level: 7),
			(name: '& Large wooden chest ';
					tval: 2; tchar: '&'; flags: $15000000; p1: 0; cost: 60;
					subval: 4; weight: 500; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d5'; level: 15),
			(name: '& Small iron chest ';
					tval: 2; tchar: '&'; flags: $0F000000; p1: 0; cost: 100;
					subval: 7; weight: 300; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d4'; level: 25),
			(name: '& Large iron chest ';
					tval: 2; tchar: '&'; flags: $1F000000; p1: 0; cost: 150;
					subval: 10; weight: 1000; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d6'; level: 35),
			(name: '& Small steel chest';
					tval: 2; tchar: '&'; flags: $0F000000; p1: 0; cost: 200;
					subval: 13; weight: 500; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d4'; level: 45),
			(name: '& Large steel chest';
					tval: 2; tchar: '&'; flags: $23000000; p1: 0; cost: 250;
					subval: 16; weight: 1000; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d6'; level: 50),
			(name: '& Rat Skeleton';
					tval: 1; tchar: 's'; flags: $00000000; p1: 0; cost:
					0; subval: 1; weight: 10; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d1'; level: 1),
			(name: '& Giant Centipede Skeleton ';
					tval: 1; tchar: 's'; flags: $00000000; p1: 0; cost:
					0; subval: 2; weight: 25; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d1'; level: 1),
			(name: 'Some filthy rags^ [%P6,%P4]';
					tval: 36; tchar: '~'; flags: $00000000; p1: 0; cost: 0;
					subval: 99; weight: 20; number: 1; tohit: 0; todam: 0;
					ac: 1; toac: 0; damage: '0d0'; level: 0),
			(name: '& empty bottle';
					tval: 1; tchar: '!'; flags: $00000000; p1: 0; cost: 0;
					subval: 4; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),
			(name: 'Some shards of pottery';
					tval: 1; tchar: '~'; flags: $00000000; p1: 0; cost: 0;
					subval: 5; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),
			(name: '& Human Skeleton';
					tval: 1; tchar: 's'; flags: $00000000; p1: 0; cost:
					0; subval: 7; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d1'; level: 1),
			(name: '& Dwarf Skeleton';
					tval: 1; tchar: 's'; flags: $00000000; p1: 0; cost:
					0; subval: 8; weight: 60; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d1'; level: 1),
			(name: '& Elf Skeleton';
					tval: 1; tchar: 's'; flags: $00000000; p1: 0; cost:
					0; subval: 9; weight: 40; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d1'; level: 1),
			(name: '& Gnome Skeleton';
					tval: 1; tchar: 's'; flags: $00000000; p1: 0; cost:
					0; subval: 10; weight: 25; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d1'; level: 1),
			(name: '& broken set of teeth';
					tval: 1; tchar: 's'; flags: $00000000; p1: 0; cost:
					0; subval: 11; weight: 3; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d1'; level: 0),
			(name: '& large broken bone';
					tval: 1; tchar: 's'; flags: $00000000; p1: 0; cost:
					0; subval: 12; weight: 2; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d1'; level: 0),
			(name: '& broken stick';
					tval: 1; tchar: '~'; flags: $00000000; p1: 0; cost: 0;
					subval: 13; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0));

//	dengland Values.inc initialised this as per the following:
//			object_ident := (max_objects of false);
//		I assume this means they should be all False, which happens anyway?
	object_ident : {[psect(moria$data)]} array [1..MAX_OBJECTS] of Boolean;

	t_level : {[psect(moria$data)]} array [0..MAX_OBJ_LEVEL] of integer;

	t_list : {[psect(moria$data)]} array [1..MAX_TALLOC] of treasure_type;
//dengland New inventory system fully implemented
//	inventory : {[psect(player$data)]} array [1..INVEN_MAX] of treasure_type;
	newInventory: TMoriaInventory;
	invenTemp: Treasure_Type;

{ Items which are sold in the stores are different from dungeon
  items so that identify works properly.  Note that the players
  recieve their initial items from this list, so position is
  very important..  }

	inventory_init : {[psect(setup$data)]} array [1..INVEN_INIT_MAX] of treasure_type = (
//dengland	Food is consumed far too quickly
//			(name: '& Ration~ of Food';
//					tval: 80; tchar: ','; flags: $00000000; p1: 5000; cost:3;
//					subval: 307; weight: 10; number: 5; tohit: 0; todam: 0;
//					ac: 0; toac: 0; damage: '0d0'; level: 0),{  1}
//			(name: '& Hard Biscuit~ ';
//					tval: 80; tchar: ','; flags: $00000000; p1: 500; cost:1;
//					subval: 309; weight: 2; number: 5; tohit: 0; todam: 0;
//					ac: 0; toac: 0; damage: '0d0'; level: 0),{  2}
//			(name: '& Strip~ of Beef Jerky';
//					tval: 80; tchar: ','; flags: $00000000; p1: 1750; cost:2;
//					subval: 310; weight: 2; number: 5; tohit: 0; todam: 0;
//					ac: 0; toac: 0; damage: '0d0'; level: 0),{  3}
			(name: '& Ration~ of Food';
					tval: 80; tchar: ','; flags: $00000000; p1: PLAYER_FOOD_FULL; cost:3;
					subval: 307; weight: 10; number: 10; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{  1}
			(name: '& Hard Biscuit~ ';
					tval: 80; tchar: ','; flags: $00000000; p1: 1000; cost:1;
					subval: 309; weight: 2; number: 5; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{  2}
			(name: '& Strip~ of Beef Jerky';
					tval: 80; tchar: ','; flags: $00000000; p1: 3000; cost:2;
					subval: 310; weight: 2; number: 5; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{  3}
			(name: '& Pint of Fine Ale ';
					tval: 80; tchar: ','; flags: $00000000; p1: 500; cost:1;
					subval: 311; weight: 10; number: 3; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{  4}
			(name: '& Pint of Fine Wine';
					tval: 80; tchar: ','; flags: $00000000; p1: 400; cost:2;
					subval: 311; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{  5}
			(name: '& Dagger <Misercorde> (%P2,%P3) ';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost:10;
					subval: 2; weight: 15; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d4'; level: 0),{  6}
			(name: '& Dagger <Stiletto> (%P2,%P3)';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost:10;
					subval: 3; weight: 12; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d4'; level: 0),{  7}
			(name: '& Bastard Sword (%P2,%P3)';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost:350;
					subval: 7; weight: 140; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d4'; level: 0),{  8}
			(name: '& Broadsword (%P2,%P3)';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost:255;
					subval: 10; weight: 150; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d5'; level: 0),{  9}
			(name: '& Longsword (%P2,%P3)';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost:300;
					subval: 18; weight: 130; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d10'; level: 0),{ 10}
			(name: '& Small Sword (%P2,%P3) ';
					tval: 23; tchar: '|'; flags: $00000000; p1: 0; cost:48;
					subval: 22; weight: 75; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d6'; level: 0),{ 11}
			(name: '& Broad Axe (%P2,%P3)';
					tval: 21; tchar: '\'; flags: $00000000; p1: 0; cost:304;
					subval: 4; weight: 160; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d5'; level: 0),{ 12}
			(name: '& Morningstar (%P2,%P3) ';
					tval: 21; tchar: '\'; flags: $00000000; p1: 0; cost:396;
					subval: 9; weight: 150; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d6'; level: 0),{ 13}
			(name: '& Mace (%P2,%P3)';
					tval: 21; tchar: '\'; flags: $00000000; p1: 0; cost:130;
					subval: 10; weight: 120; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d4'; level: 0),{ 14}
			(name: '& War Hammer (%P2,%P3)';
					tval: 21; tchar: '\'; flags: $00000000; p1: 0; cost:225;
					subval: 11; weight: 120; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d3'; level: 0),{ 15}
			(name: '& Halberd (%P2,%P3)';
					tval: 22; tchar: '/'; flags: $00000000; p1: 0; cost:430;
					subval: 5; weight: 190; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d4'; level: 0),{ 16}
			(name: '& Pike (%P2,%P3)';
					tval: 22; tchar: '/'; flags: $00000000; p1: 0; cost:358;
					subval: 7; weight: 160; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d5'; level: 0),{ 17}
			(name: '& Spear (%P2,%P3)';
					tval: 22; tchar: '/'; flags: $00000000; p1: 0; cost:36;
					subval: 8; weight: 50; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d6'; level: 0),{ 18}
			(name: '& Short Bow (%P2)';
					tval: 20; tchar: '}'; flags: $00000000; p1: 2; cost:50;
					subval: 1; weight: 30; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 19}
			(name: '& Long Bow (%P2)';
					tval: 20; tchar: '}'; flags: $00000000; p1: 3; cost:120;
					subval: 2; weight: 40; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 20}
			(name: '& Light Crossbow (%P2)';
					tval: 20; tchar: '}'; flags: $00000000; p1: 5; cost:160;
					subval: 10; weight: 110; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 21}
			(name: '& Sling (%P2)';
					tval: 20; tchar: '}'; flags: $00000000; p1: 1; cost:5;
					subval: 20; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 22}
			(name: '& Arrow~ (%P2,%P3) ';
					tval: 12; tchar: '{'; flags: $00000000; p1: 0; cost:1;
					subval: 1; weight: 2; number: 40; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d4'; level: 0),{ 23}
			(name: '& Bolt~ (%P2,%P3)';
					tval: 11; tchar: '{'; flags: $00000000; p1: 0; cost:2;
					subval: 1; weight: 3; number: 40; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d5'; level: 0),{ 24}
			(name: '& Iron Shot~ (%P2,%P3)';
					tval: 10; tchar: '{'; flags: $00000000; p1: 0; cost:2;
					subval: 1; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d3'; level: 0),{ 25}
			(name: '& Pick (%P1) (%P2,%P3)';
					tval: 25; tchar: '\'; flags: $20000000; p1: 1; cost:50;
					subval: 1; weight: 150; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d3'; level: 0),{ 26}
			(name: '& Shovel (%P1) (%P2,%P3)';
					tval: 25; tchar: '\'; flags: $20000000; p1: 0; cost:15;
					subval: 2; weight: 60; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d2'; level: 0),{ 27}
			(name: '& Pair of Soft Leather Boots [%P6,%P4]';
					tval: 30; tchar: ']'; flags: $00000000; p1: 0; cost:7;
					subval: 2; weight: 30; number: 1; tohit: 0; todam: 0;
					ac: 2; toac: 0; damage: '1d1'; level: 0),{ 28}
			(name: '& Pair of Hard Leather Boots [%P6,%P4]';
					tval: 30; tchar: ']'; flags: $00000000; p1: 0; cost:12;
					subval: 3; weight: 40; number: 1; tohit: 0; todam: 0;
					ac: 3; toac: 0; damage: '1d1'; level: 0),{ 29}
			(name: '& Hard Leather Cap [%P6,%P4] ';
					tval: 33; tchar: ']'; flags: $00000000; p1: 0; cost:12;
					subval: 2; weight: 15; number: 1; tohit: 0; todam: 0;
					ac: 2; toac: 0; damage: '0d0'; level: 0),{ 30}
			(name: '& Metal Cap [%P6,%P4]';
					tval: 33; tchar: ']'; flags: $00000000; p1: 0; cost:30;
					subval: 3; weight: 20; number: 1; tohit: 0; todam: 0;
					ac: 3; toac: 0; damage: '1d1'; level: 0),{ 31}
			(name: '& Iron Helm [%P6,%P4]';
					tval: 33; tchar: ']'; flags: $00000000; p1: 0; cost:75;
					subval: 4; weight: 75; number: 1; tohit: 0; todam: 0;
					ac: 5; toac: 0; damage: '1d3'; level: 0),{ 32}
			(name: 'Soft Leather Armor [%P6,%P4] ';
					tval: 36; tchar: '('; flags: $00000000; p1: 0; cost:18;
					subval: 2; weight: 80; number: 1; tohit: 0; todam: 0;
					ac: 4; toac: 0; damage: '0d0'; level: 0),{ 33}
			(name: 'Soft Studded Leather [%P6,%P4]';
					tval: 36; tchar: '('; flags: $00000000; p1: 0; cost:35;
					subval: 3; weight: 90; number: 1; tohit: 0; todam: 0;
					ac: 5; toac: 0; damage: '1d1'; level: 0),{ 34}
			(name: 'Hard Leather Armor [%P6,%P4] ';
					tval: 36; tchar: '('; flags: $00000000; p1: 0; cost:55;
					subval: 4; weight: 100; number: 1; tohit: -1; todam: 0;
					ac: 6; toac: 0; damage: '1d1'; level: 0),{ 35}
			(name: 'Hard Studded Leather [%P6,%P4]';
					tval: 36; tchar: '('; flags: $00000000; p1: 0; cost:100;
					subval: 5; weight: 110; number: 1; tohit: -1; todam: 0;
					ac: 7; toac: 0; damage: '1d2'; level: 0),{ 36}
			(name: 'Leather Scale Mail [%P6,%P4] ';
					tval: 36; tchar: '('; flags: $00000000; p1: 0; cost:330;
					subval: 9; weight: 140; number: 1; tohit: -1; todam: 0;
					ac: 11; toac: 0; damage: '1d1'; level: 0),{ 37}
			(name: 'Metal Scale Mail [%P6,%P4] ';
					tval: 35; tchar: '['; flags: $00000000; p1: 0; cost:430;
					subval: 1; weight: 250; number: 1; tohit: -2; todam: 0;
					ac: 13; toac: 0; damage: '1d4'; level: 0),{ 38}
			(name: 'Chain Mail [%P6,%P4] ';
					tval: 35; tchar: '['; flags: $00000000; p1: 0; cost:530;
					subval: 2; weight: 220; number: 1; tohit: -2; todam: 0;
					ac: 14; toac: 0; damage: '1d4'; level: 0),{ 39}
			(name: 'Partial Plate Armor [%P6,%P4]';
					tval: 35; tchar: '['; flags: $00000000; p1: 0; cost:900;
					subval: 9; weight: 260; number: 1; tohit: -3; todam: 0;
					ac: 22; toac: 0; damage: '1d6'; level: 0),{ 40}
			(name: 'Full Plate Armor [%P6,%P4] ';
					tval: 35; tchar: '['; flags: $00000000; p1: 0; cost:1050;
					subval: 11; weight: 380; number: 1; tohit: -3; todam: 0;
					ac: 25; toac: 0; damage: '2d4'; level: 0),{ 41}
			(name: '& Cloak [%P6,%P4]';
					tval: 32; tchar: '('; flags: $00000000; p1: 0; cost:3;
					subval: 1; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 1; toac: 0; damage: '0d0'; level: 0),{ 42}
			(name: '& Set of Leather Gloves [%P6,%P4]';
					tval: 31; tchar: ']'; flags: $00000000; p1: 0; cost:3;
					subval: 1; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 1; toac: 0; damage: '0d0'; level: 0),{ 43}
			(name: '& Set of Gauntlets [%P6,%P4] ';
					tval: 31; tchar: ']'; flags: $00000000; p1: 0; cost:35;
					subval: 2; weight: 25; number: 1; tohit: 0; todam: 0;
					ac: 2; toac: 0; damage: '1d1'; level: 0),{ 44}
			(name: '& Small Leather Shield [%P6,%P4]';
					tval: 34; tchar: ')'; flags: $00000000; p1: 0; cost:30;
					subval: 1; weight: 50; number: 1; tohit: 0; todam: 0;
					ac: 2; toac: 0; damage: '1d1'; level: 0),{ 45}
			(name: '& Medium Leather Shield [%P6,%P4]';
					tval: 34; tchar: ')'; flags: $00000000; p1: 0; cost:60;
					subval: 2; weight: 75; number: 1; tohit: 0; todam: 0;
					ac: 3; toac: 0; damage: '1d2'; level: 0),{ 46}
			(name: '& Small Metal Shield [%P6,%P4]';
					tval: 34; tchar: ')'; flags: $00000000; p1: 0; cost:50;
					subval: 4; weight: 65; number: 1; tohit: 0; todam: 0;
					ac: 3; toac: 0; damage: '1d3'; level: 0),{ 47}
			(name: '& Ring of Resist Fire';
					tval: 45; tchar: '='; flags: $00080000; p1: 0; cost:250;
					subval: 11; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 48}
			(name: '& Ring of Resist Cold';
					tval: 45; tchar: '='; flags: $00200000; p1: 0; cost:250;
					subval: 12; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 49}
			(name: '& Ring of Feather Falling';
					tval: 45; tchar: '='; flags: $04000000; p1: 0; cost:250;
					subval: 13; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 50}
			(name: '& Ring of Protection [%P4] ';
					tval: 45; tchar: '='; flags: $00000000; p1: 0; cost:100;
					subval: 24; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 51}
			(name: '& Amulet of Charisma (%P1) ';
					tval: 40; tchar: '"'; flags: $00000020; p1: 0; cost:250;
					subval: 6; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 52}
			(name: '& Amulet of Slow Digestion ';
					tval: 40; tchar: '"'; flags: $00000080; p1: 0; cost:200;
					subval: 9; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 53}
			(name: '& Amulet of Resist Acid ';
					tval: 40; tchar: '"'; flags: $00100000; p1: 0; cost:300;
					subval: 10; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 54}
			(name: '& Scroll~ of Enchant Weapon To-Hit ';
					tval: 70; tchar: '?'; flags: $00000001; p1: 0; cost:125;
					subval: 300; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 55}
			(name: '& Scroll~ of Enchant Weapon To-Dam ';
					tval: 70; tchar: '?'; flags: $00000002; p1: 0; cost:125;
					subval: 301; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 56}
			(name: '& Scroll~ of Enchant Armor ';
					tval: 70; tchar: '?'; flags: $00000004; p1: 0; cost:125;
					subval: 302; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 57}
			(name: '& Scroll~ of Identify';
					tval: 70; tchar: '?'; flags: $00000008; p1: 0; cost:50;
					subval: 303; weight: 5; number: 3; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 58}
			(name: '& Scroll~ of Remove Curse';
					tval: 70; tchar: '?'; flags: $00000010; p1: 0; cost:100;
					subval: 304; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 59}
			(name: '& Scroll~ of Light ';
					tval: 70; tchar: '?'; flags: $00000020; p1: 0; cost:15;
					subval: 305; weight: 5; number: 3; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 60}
			(name: '& Scroll~ of Phase Door ';
					tval: 70; tchar: '?'; flags: $00000080; p1: 0; cost:15;
					subval: 306; weight: 5; number: 2; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 61}
			(name: '& Scroll~ of Magic Mapping ';
					tval: 70; tchar: '?'; flags: $00000800; p1: 0; cost:40;
					subval: 307; weight: 5; number: 2; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 62}
			(name: '& Scroll~ of Treasure Detection ';
					tval: 70; tchar: '?'; flags: $00004000; p1: 0; cost:15;
					subval: 308; weight: 5; number: 2; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 63}
			(name: '& Scroll~ of Object Detection';
					tval: 70; tchar: '?'; flags: $00008000; p1: 0; cost:15;
					subval: 309; weight: 5; number: 2; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 64}
			(name: '& Scroll~ of Detect Invisible';
					tval: 70; tchar: '?'; flags: $00080000; p1: 0; cost:15;
					subval: 310; weight: 5; number: 2; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 65}
			(name: '& Scroll~ of Recharging ';
					tval: 70; tchar: '?'; flags: $01000000; p1: 0; cost:200;
					subval: 311; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 66}
			(name: '& Book of Magic Spells [Beginners-Magik]';
					tval: 90; tchar: '?'; flags: $0000007F; p1: 0; cost:25;
					subval: 257; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 67}
			(name: '& Book of Magic Spells [Magik I]';
					tval: 90; tchar: '?'; flags: $0000FF80; p1: 0; cost:100;
					subval: 258; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 68}
			(name: '& Book of Magic Spells [Magik II]';
					tval: 90; tchar: '?'; flags: $00FF0000; p1: 0; cost:400;
					subval: 259; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 69}
			(name: '& Book of Magic Spells [The Mages Guide to Power]';
					tval: 90; tchar: '?'; flags: $7F000000; p1: 0; cost:800;
					subval: 261; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 70}
			(name: '& Holy Book of Prayers [Beginners Handbook]';
					tval: 91; tchar: '?'; flags: $000000FF; p1: 0; cost:25;
					subval: 258; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 71}
			(name: '& Holy Book of Prayers [Words of Wisdom]';
					tval: 91; tchar: '?'; flags: $0000FF00; p1: 0; cost:100;
					subval: 259; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 72}
			(name: '& Holy Book of Prayers [Chants and Blessings]';
					tval: 91; tchar: '?'; flags: $01FF0000; p1: 0; cost:300;
					subval: 260; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 73}
			(name: '& Holy Book of Prayers [Exorcism and Dispelling]';
					tval: 91; tchar: '?'; flags: $7E000000; p1: 0; cost:900;
					subval: 261; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 74}
			(name: '& Potion~ of Restore Strength';
					tval: 75; tchar: '!'; flags: $00000004; p1: 0; cost:300;
					subval: 310; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 75}
			(name: '& Potion~ of Restore Intelligence';
					tval: 75; tchar: '!'; flags: $00000020; p1: 0; cost:300;
					subval: 311; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 76}
			(name: '& Potion~ of Restore Wisdom';
					tval: 75; tchar: '!'; flags: $00000100; p1: 0; cost:300;
					subval: 312; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 77}
			(name: '& Potion~ of Restore Charisma';
					tval: 75; tchar: '!'; flags: $00000800; p1: 0; cost:300;
					subval: 313; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 78}
			(name: '& Potion~ of Cure Light Wounds';
					tval: 75; tchar: '!'; flags: $10001000; p1: 0; cost:15;
					subval: 314; weight: 4; number: 2; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 79}
			(name: '& Potion~ of Cure Serious Wounds';
					tval: 75; tchar: '!'; flags: $30002000; p1: 0; cost:40;
					subval: 315; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 80}
			(name: '& Potion~ of Cure Critical Wounds';
					tval: 75; tchar: '!'; flags: $70004000; p1: 0; cost:100;
					subval: 316; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 81}
			(name: '& Potion~ of Restore Dexterity';
					tval: 75; tchar: '!'; flags: $04000000; p1: 0; cost:300;
					subval: 317; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 82}
			(name: '& Potion~ of Restore Constitution';
					tval: 75; tchar: '!'; flags: $68000000; p1: 0; cost:300;
					subval: 318; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 83}
			(name: '& Potion~ of Heroism';
					tval: 76; tchar: '!'; flags: $00000010; p1: 0; cost:35;
					subval: 319; weight: 4; number: 2; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 84}
			(name: '& Potion~ of Boldliness';
					tval: 76; tchar: '!'; flags: $00000040; p1: 0; cost:10;
					subval: 320; weight: 4; number: 2; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 85}
			(name: '& Wand of Light (%P1 charges)';
					tval: 65; tchar: '-'; flags: $00000001; p1: 0; cost:200;
					subval: 1; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 2),{ 86}
			(name: '& Wand of Lightning Bolts (%P1 charges)';
					tval: 65; tchar: '-'; flags: $00000002; p1: 0; cost:600;
					subval: 2; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 6),{ 87}
			(name: '& Wand of Magic Missle (%P1 charges)';
					tval: 65; tchar: '-'; flags: $00002000; p1: 0; cost:200;
					subval: 14; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 2),{ 88}
			(name: '& Wand of Disarming (%P1 charges)';
					tval: 65; tchar: '-'; flags: $00020000; p1: 0; cost:700;
					subval: 18; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 12),{ 89}
			(name: '& Wand of Lightning Balls (%P1 charges) ';
					tval: 65; tchar: '-'; flags: $00040000; p1: 0; cost:1200;
					subval: 19; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 20),{ 90}
			(name: '& Wand of Wonder (%P1 charges)';
					tval: 65; tchar: '-'; flags: $00200000; p1: 0; cost:250;
					subval: 22; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 10),{ 91}
			(name: '& Staff of Light (%P1 charges)';
					tval: 55; tchar: '_'; flags: $00000001; p1: 0; cost:
					250; subval: 1; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 3),{ 92}
			(name: '& Staff of Door/Stair Location (%P1 charges)';
					tval: 55; tchar: '_'; flags: $00000002; p1: 0; cost:
					350; subval: 2; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 7),{ 93}
			(name: '& Staff of Trap Location (%P1 charges)';
					tval: 55; tchar: '_'; flags: $00000004; p1: 0; cost:
					350; subval: 3; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 7),{ 94}
			(name: '& Staff of Detect Invisible (%P1 charges)';
					tval: 55; tchar: '_'; flags: $00008000; p1: 0; cost:
					200; subval: 16; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 3),{ 95}
			(name: '& Potion~ of Restore Life Levels';
					tval: 76; tchar: '!'; flags: $00000080; p1: 0; cost:400;
					subval: 321; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 96}
			(name: '& Scroll~ of Blessing';
					tval: 71; tchar: '?'; flags: $00000020; p1: 0; cost:15;
					subval: 312; weight: 5; number: 2; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 97}
			(name: '& Scroll~ of Word-of-Recall';
					tval: 71; tchar: '?'; flags: $00000100; p1: 0; cost:150;
					subval: 265; weight: 5; number: 3; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 0),{ 98}
			(name: '& Potion~ of Slow Poison';
					tval: 76; tchar: '!'; flags: $00000800; p1: 0; cost:25;
					subval: 322; weight: 4; number: 2; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{ 99}
			(name: '& Potion~ of Neutralize Poison';
					tval: 76; tchar: '!'; flags: $00001000; p1: 0; cost:75;
					subval: 323; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0),{100}
			(name: '& Wand of Stinking Cloud (%P1 charges)';
					tval: 65; tchar: '-'; flags: $00200000; p1: 10; cost:400;
					subval: 21; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 5),{101}
			(name: '& Iron Spike~';
					tval: 13; tchar: '~'; flags: $00000000; p1: 0; cost:1;
					subval: 1; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 1),{102}
			(name: '& Brass Lantern~ with %P5 turns of light';
					tval: 15; tchar: '~'; flags: $00000000; p1: 7500; cost:35;
					subval: 2; weight: 50; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 1),{103}
			(name: '& Wooden Torch~ with %P5 turns of light ';
					tval: 15; tchar: '~'; flags: $00000000; p1: 4000; cost:2;
					subval: 270; weight: 30; number: 10; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 1),{104}
			(name: '& Flask~ of oil ';
					tval: 77; tchar: '!'; flags: $00040000; p1: 7500; cost:3;
					subval: 257; weight: 10; number: 5; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d6'; level: 1),{105}
			(name: '& Book of extra Spells [Consultants-Magik]';
					tval: 92; tchar: '?'; flags: $00000FFF; p1: 0; cost:25;
					subval: 257; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 0));{106}


	blk_mkt_init : {[psect(setup$data)]} array [1..BLK_MKT_MAX] of treasure_type = (
//			(name: '& Ration~ of Food';
//					tval:80; tchar: ','; flags: $00000000; p1: 5000; cost: 30;
//					subval: 307; weight: 10; number: 5; tohit: 0; todam: 0;
//					ac: 0; toac: 0; damage: '0d0'; level: 50), {  1}
//			(name: '& Piece~ of Elvish Waybread';
//					tval:80; tchar: ','; flags: $02000020; p1: 8000; cost: 100;
//					subval: 311; weight: 3; number: 2; tohit: 0; todam: 0;
//					ac: 0; toac: 0; damage: '0d0'; level: 50), {  2}
			(name: '& Ration~ of Food';
					tval:80; tchar: ','; flags: $00000000; p1: PLAYER_FOOD_FULL; cost: 30;
					subval: 307; weight: 10; number: 5; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), {  1}
			(name: '& Piece~ of Elvish Waybread';
					tval:80; tchar: ','; flags: $02000020; p1: PLAYER_FOOD_MAX; cost: 100;
					subval: 311; weight: 3; number: 2; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), {  2}
			(name: '& Mushroom~ of Restoring';
					tval:80; tchar: ','; flags: $001F8040; p1: 1000; cost: 10000;
					subval: 272; weight: 1; number: 2; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), {  3}
			(name: '& Mushroom~ of Restore Constitution';
					tval:80; tchar: ','; flags: $00010000; p1: 1000; cost: 700;
					subval: 268; weight: 1; number: 2; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), {  4}
			(name: '& Dagger <Main Gauche> (%P2,%P3)';
					tval:23; tchar: '|'; flags: $00000000; p1: 0; cost: 25000;
					subval: 1; weight: 30; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d5'; level: 50), {  5}
			(name: '& Bastard Sword (%P2,%P3)';
					tval:23; tchar: '|'; flags: $00000000; p1: 0; cost: 35000;
					subval: 7; weight: 140; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d4'; level: 50), {  6}
			(name: '& Broadsword (%P2,%P3)';
					tval:23; tchar: '|'; flags: $00000000; p1: 0; cost: 25500;
					subval: 10; weight: 150; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d5'; level: 50), {  7}
			(name: '& Two Handed Sword <Claymore> (%P2,%P3)';
					tval:23; tchar: '|'; flags: $00000000; p1: 0; cost: 77500;
					subval: 11; weight: 200; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d6'; level: 50), {  8}
			(name: '& Two Handed Sword <Espadon> (%P2,%P3)';
					tval:23; tchar: '|'; flags: $00000000; p1: 0; cost: 67500;
					subval: 13; weight: 180; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d6'; level: 50), {  9}
			(name: '& Executioner''s Sword (%P2,%P3)';
					tval:23; tchar: '|'; flags: $00000000; p1: 0; cost: 85000;
					subval: 14; weight: 260; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '4d5'; level: 50), { 10}
			(name: '& Two Handed Sword <Flamberge> (%P2,%P3)';
					tval:23; tchar: '|'; flags: $00000000; p1: 0; cost: 100000;
					subval: 15; weight: 240; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '4d5'; level: 50), { 11}
			(name: '& Longsword (%P2,%P3)';
					tval:23; tchar: '|'; flags: $00000000; p1: 0; cost: 30000;
					subval: 18; weight: 130; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d10'; level: 50), { 12}
			(name: '& Two Handed Sword <No-Dachi> (%P2,%P3)';
					tval:23; tchar: '|'; flags: $00000000; p1: 0; cost: 67500;
					subval: 19; weight: 200; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '4d4'; level: 50), { 13}
			(name: '& Small Sword (%P2,%P3)';
					tval:23; tchar: '|'; flags: $00000000; p1: 0; cost: 4800;
					subval: 22; weight: 75; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d6'; level: 50), { 14}
			(name: '& Two Handed Sword <Zweihander> (%P2,%P3)';
					tval:23; tchar: '|'; flags: $00000000; p1: 0; cost: 100000;
					subval: 23; weight: 280; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '4d6'; level: 50), { 15}
			(name: '& Battle Axe <Balestarius> (%P2,%P3)';
					tval:21; tchar: '\'; flags: $00000000; p1: 0; cost: 50000;
					subval: 1; weight: 180; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d8'; level: 50), { 16}
			(name: '& Battle Axe <European> (%P2,%P3)';
					tval:21; tchar: '\'; flags: $00000000; p1: 0; cost: 33400;
					subval: 3; weight: 170; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d4'; level: 50), { 17}
			(name: '& Broad Axe (%P2,%P3)';
					tval:21; tchar: '\'; flags: $00000000; p1: 0; cost: 30400;
					subval: 4; weight: 160; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d6'; level: 50), { 18}
			(name: '& Two Handed Great Flail (%P2,%P3)';
					tval:21; tchar: '\'; flags: $00000000; p1: 0; cost: 59000;
					subval: 8; weight: 280; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d6'; level: 50), { 19}
			(name: '& Morningstar (%P2,%P3)';
					tval:21; tchar: '\'; flags: $00000000; p1: 0; cost: 39600;
					subval: 9; weight: 150; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d6'; level: 50), { 20}
			(name: '& Mace (%P2,%P3)';
					tval:21; tchar: '\'; flags: $00000000; p1: 0; cost: 13000;
					subval: 10; weight: 120; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d4'; level: 50), { 21}
			(name: '& War Hammer (%P2,%P3)';
					tval:21; tchar: '\'; flags: $00000000; p1: 0; cost: 22500;
					subval: 11; weight: 120; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d3'; level: 50), { 22}
			(name: '& Mace <Lead filled> (%P2,%P3)';
					tval:21; tchar: '\'; flags: $00000000; p1: 0; cost: 50200;
					subval: 12; weight: 180; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d4'; level: 50), { 23}
			(name: '& Beaked Axe (%P2,%P3)';
					tval:22; tchar: '/'; flags: $00000000; p1: 0; cost: 40800;
					subval: 2; weight: 180; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d6'; level: 50), { 24}
			(name: '& Glaive (%P2,%P3)';
					tval:22; tchar: '/'; flags: $00000000; p1: 0; cost: 36300;
					subval: 4; weight: 190; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d6'; level: 50), { 25}
			(name: '& Halberd (%P2,%P3)';
					tval:22; tchar: '/'; flags: $00000000; p1: 0; cost: 43000;
					subval: 5; weight: 190; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '3d4'; level: 50), { 26}
			(name: '& Lucerne Hammer (%P2,%P3)';
					tval:22; tchar: '/'; flags: $00000000; p1: 0; cost: 37600;
					subval: 6; weight: 120; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d5'; level: 50), { 27}
			(name: '& Pike (%P2,%P3)';
					tval:22; tchar: '/'; flags: $00000000; p1: 0; cost: 35800;
					subval: 7; weight: 160; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d5'; level: 50), { 28}
			(name: '& Lance (%P2,%P3)';
					tval:22; tchar: '/'; flags: $00000000; p1: 0; cost: 23000;
					subval: 9; weight: 300; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d8'; level: 50), { 29}
			(name: '& Short Bow (%P2)';
					tval:20; tchar: '}'; flags: $00000000; p1: 2; cost: 5000;
					subval: 1; weight: 30; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), { 30}
			(name: '& Long Bow (%P2)';
					tval:20; tchar: '}'; flags: $00000000; p1: 3; cost: 12000;
					subval: 2; weight: 40; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), { 31}
			(name: '& Composite Bow (%P2)';
					tval:20; tchar: '}'; flags: $00000000; p1: 4; cost: 24000;
					subval: 3; weight: 40; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), { 32}
			(name: '& Light Crossbow (%P2)';
					tval:20; tchar: '}'; flags: $00000000; p1: 5; cost: 14000;
					subval: 10; weight: 110; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), { 33}
			(name: '& Heavy Crossbow (%P2)';
					tval:20; tchar: '}'; flags: $00000000; p1: 6; cost: 30000;
					subval: 11; weight: 200; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), { 34}
			(name: '& Sling (%P2)';
					tval:20; tchar: '}'; flags: $00000000; p1: 1; cost: 500;
					subval: 20; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 35}
			(name: '& Arrow~ (%P2,%P3)';
					tval:12; tchar: '{'; flags: $00000000; p1: 0; cost: 100;
					subval: 1; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d4'; level: 50), { 36}
			(name: '& Bolt~ (%P2,%P3)';
					tval:11; tchar: '{'; flags: $00000000; p1: 0; cost: 200;
					subval: 1; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d5'; level: 50), { 37}
			(name: '& Rounded Pebble~ (%P2,%P3)';
					tval:10; tchar: '{'; flags: $00000000; p1: 0; cost: 100;
					subval: 1; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d2'; level: 50), { 38}
			(name: '& Iron Shot~ (%P2,%P3)';
					tval:10; tchar: '{'; flags: $00000000; p1: 0; cost: 200;
					subval: 1; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d3'; level: 50), { 39}
			(name: '& Brass Lantern~ with %P5 turns of light';
					tval:15; tchar: '~'; flags: $00000000; p1: 7500; cost: 3500;
					subval: 1; weight: 50; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), { 40}
			(name: '& Orcish Pick (%P1) (%P2,%P3)';
					tval:25; tchar: '\'; flags: $20000000; p1: 2; cost: 50000;
					subval: 2; weight: 180; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d5'; level: 50), { 41}
			(name: '& Dwarven Pick (%P1) (%P2,%P3)';
					tval:25; tchar: '\'; flags: $20000000; p1: 3; cost: 120000;
					subval: 3; weight: 200; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d5'; level: 50), { 42}
			(name: '& Gnomish Shovel (%P1) (%P2,%P3)';
					tval:25; tchar: '\'; flags: $20000000; p1: 1; cost: 10000;
					subval: 5; weight: 50; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d2'; level: 50), { 43}
			(name: '& Dwarven Shovel (%P1) (%P2,%P3)';
					tval:25; tchar: '\'; flags: $20000000; p1: 2; cost: 25000;
					subval: 6; weight: 120; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d3'; level: 50), { 44}
			(name: '& Pair of Hard Leather Boots [%P6,%P4]';
					tval:30; tchar: ']'; flags: $00000000; p1: 0; cost: 1200;
					subval: 3; weight: 40; number: 1; tohit: 0; todam: 0;
					ac: 3; toac: 0; damage: '1d1'; level: 50), { 45}
			(name: '& Hard Leather Cap [%P6,%P4]';
					tval:33; tchar: ']'; flags: $00000000; p1: 0; cost: 1200;
					subval: 2; weight: 15; number: 1; tohit: 0; todam: 0;
					ac: 2; toac: 0; damage: '0d0'; level: 50), { 46}
			(name: '& Metal Cap [%P6,%P4]';
					tval:33; tchar: ']'; flags: $00000000; p1: 0; cost: 3000;
					subval: 3; weight: 20; number: 1; tohit: 0; todam: 0;
					ac: 3; toac: 0; damage: '1d1'; level: 50), { 47}
			(name: '& Iron Helm [%P6,%P4]';
					tval:33; tchar: ']'; flags: $00000000; p1: 0; cost: 7500;
					subval: 4; weight: 75; number: 1; tohit: 0; todam: 0;
					ac: 5; toac: 0; damage: '1d3'; level: 50), { 48}
			(name: '& Steel Helm [%P6,%P4]';
					tval:33; tchar: ']'; flags: $00000000; p1: 0; cost: 20000;
					subval: 5; weight: 60; number: 1; tohit: 0; todam: 0;
					ac: 6; toac: 0; damage: '1d3'; level: 50), { 49}
			(name: '& Silver Crown (%P1)';
					tval:33; tchar: ']'; flags: $00000000; p1: 0; cost: 25000;
					subval: 6; weight: 20; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), { 50}
			(name: '& Golden Crown (%P1)';
					tval:33; tchar: ']'; flags: $00000000; p1: 0; cost: 50000;
					subval: 7; weight: 30; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), { 51}
			(name: '& Jewel Encrusted Crown (%P1)';
					tval:33; tchar: ']'; flags: $00000000; p1: 0; cost: 100000;
					subval: 8; weight: 40; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), { 52}
			(name: 'Double Chain Mail [%P6,%P4]';
					tval:35; tchar: '['; flags: $00000000; p1: 0; cost: 63000;
					subval: 4; weight: 260; number: 1; tohit: -2; todam: 0;
					ac: 15; toac: 0; damage: '1d4'; level: 50), { 53}
			(name: 'Augmented Chain Mail [%P6,%P4]';
					tval:35; tchar: '['; flags: $00000000; p1: 0; cost: 67500;
					subval: 5; weight: 270; number: 1; tohit: -2; todam: 0;
					ac: 16; toac: 0; damage: '1d4'; level: 50), { 54}
			(name: 'Bar Chain Mail [%P6,%P4]';
					tval:35; tchar: '['; flags: $00000000; p1: 0; cost: 72000;
					subval: 6; weight: 280; number: 1; tohit: -2; todam: 0;
					ac: 18; toac: 0; damage: '1d4'; level: 50), { 55}
			(name: 'Metal Brigandine Armor [%P6,%P4]';
					tval:35; tchar: '['; flags: $00000000; p1: 0; cost: 77500;
					subval: 7; weight: 290; number: 1; tohit: -3; todam: 0;
					ac: 19; toac: 0; damage: '1d4'; level: 50), { 56}
			(name: 'Laminated Armor [%P6,%P4]';
					tval:35; tchar: '['; flags: $00000000; p1: 0; cost: 82500;
					subval: 8; weight: 300; number: 1; tohit: -3; todam: 0;
					ac: 20; toac: 0; damage: '1d4'; level: 50), { 57}
			(name: 'Partial Plate Armor [%P6,%P4]';
					tval:35; tchar: '['; flags: $00000000; p1: 0; cost: 90000;
					subval: 9; weight: 260; number: 1; tohit: -3; todam: 0;
					ac: 22; toac: 0; damage: '1d6'; level: 50), { 58}
			(name: 'Metal Lamellar Armor [%P6,%P4]';
					tval:35; tchar: '['; flags: $00000000; p1: 0; cost: 95000;
					subval: 10; weight: 340; number: 1; tohit: -3; todam: 0;
					ac: 23; toac: 0; damage: '1d6'; level: 50), { 59}
			(name: 'Full Plate Armor [%P6,%P4]';
					tval:35; tchar: '['; flags: $00000000; p1: 0; cost: 105000;
					subval: 11; weight: 380; number: 1; tohit: -3; todam: 0;
					ac: 25; toac: 0; damage: '2d4'; level: 50), { 60}
			(name: 'Ribbed Plate Armor [%P6,%P4]';
					tval:35; tchar: '['; flags: $00000000; p1: 0; cost: 120000;
					subval: 12; weight: 380; number: 1; tohit: -3; todam: 0;
					ac: 28; toac: 0; damage: '2d4'; level: 50), { 61}
			(name: '& Cloak [%P6,%P4]';
					tval:32; tchar: '('; flags: $00000000; p1: 0; cost: 300;
					subval: 1; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 1; toac: 0; damage: '0d0'; level: 50), { 62}
			(name: '& Set of Leather Gloves [%P6,%P4]';
					tval:31; tchar: ']'; flags: $00000000; p1: 0; cost: 300;
					subval: 1; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 1; toac: 0; damage: '0d0'; level: 50), { 63}
			(name: '& Set of Gauntlets [%P6,%P4]';
					tval:31; tchar: ']'; flags: $00000000; p1: 0; cost: 3500;
					subval: 2; weight: 25; number: 1; tohit: 0; todam: 0;
					ac: 2; toac: 0; damage: '1d1'; level: 50), { 64}
			(name: '& Small Leather Shield [%P6,%P4]';
					tval:34; tchar: ')'; flags: $00000000; p1: 0; cost: 3000;
					subval: 1; weight: 50; number: 1; tohit: 0; todam: 0;
					ac: 2; toac: 0; damage: '1d1'; level: 50), { 65}
			(name: '& Medium Leather Shield [%P6,%P4]';
					tval:34; tchar: ')'; flags: $00000000; p1: 0; cost: 6000;
					subval: 2; weight: 75; number: 1; tohit: 0; todam: 0;
					ac: 3; toac: 0; damage: '1d2'; level: 50), { 66}
			(name: '& Large Leather Shield [%P6,%P4]';
					tval:34; tchar: ')'; flags: $00000000; p1: 0; cost: 12000;
					subval: 3; weight: 100; number: 1; tohit: 0; todam: 0;
					ac: 4; toac: 0; damage: '1d2'; level: 50), { 67}
			(name: '& Small Metal Shield [%P6,%P4]';
					tval:34; tchar: ')'; flags: $00000000; p1: 0; cost: 5000;
					subval: 4; weight: 65; number: 1; tohit: 0; todam: 0;
					ac: 3; toac: 0; damage: '1d2'; level: 50), { 68}
			(name: '& Medium Metal Shield [%P6,%P4]';
					tval:34; tchar: ')'; flags: $00000000; p1: 0; cost: 12500;
					subval: 5; weight: 90; number: 1; tohit: 0; todam: 0;
					ac: 4; toac: 0; damage: '1d3'; level: 50), { 69}
			(name: '& Large Metal Shield [%P6,%P4]';
					tval:34; tchar: ')'; flags: $00000000; p1: 0; cost: 20000;
					subval: 6; weight: 120; number: 1; tohit: 0; todam: 0;
					ac: 5; toac: 0; damage: '1d3'; level: 50), { 70}
			(name: '& Ring of Gain Strength (%P1)';
					tval:45; tchar: '='; flags: $00000001; p1: 0; cost: 40000;
					subval: 1; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 71}
			(name: '& Ring of Gain Dexterity (%P1)';
					tval:45; tchar: '='; flags: $00000002; p1: 0; cost: 40000;
					subval: 2; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 72}
			(name: '& Ring of Gain Constitution (%P1)';
					tval:45; tchar: '='; flags: $00000004; p1: 0; cost: 40000;
					subval: 3; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 73}
			(name: '& Ring of Gain Intelligence (%P1)';
					tval:45; tchar: '='; flags: $00000008; p1: 0; cost: 35000;
					subval: 4; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 74}
			(name: '& Ring of Speed (%P1)';
					tval:45; tchar: '='; flags: $00001000; p1: 0; cost: 300000;
					subval: 7; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 75}
			(name: '& Ring of Searching (%P1)';
					tval:45; tchar: '='; flags: $00000040; p1: 0; cost: 25000;
					subval: 8; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 76}
			(name: '& Ring of Slow Digestion';
					tval:45; tchar: '='; flags: $00000080; p1: 0; cost: 25000;
					subval: 10; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 77}
			(name: '& Ring of Feather Falling';
					tval:45; tchar: '='; flags: $04000000; p1: 0; cost: 20000;
					subval: 13; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 78}
			(name: '& Ring of Lordly Protection (FIRE)';
					tval:45; tchar: '='; flags: $00080000; p1: 0; cost: 120000;
					subval: 17; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 5; damage: '0d0'; level: 50), { 79}
			(name: '& Ring of Lordly Protection (ACID)';
					tval:45; tchar: '='; flags: $00100000; p1: 0; cost: 120000;
					subval: 18; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 5; damage: '0d0'; level: 50), { 80}
			(name: '& Ring of Lordly Protection (COLD)';
					tval:45; tchar: '='; flags: $00200000; p1: 0; cost: 120000;
					subval: 19; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 5; damage: '0d0'; level: 50), { 81}
			(name: '& Ring of Increase Damage (%P3)';
					tval:45; tchar: '='; flags: $00000000; p1: 0; cost: 10000;
					subval: 22; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 82}
			(name: '& Ring of Increase To-Hit (%P2)';
					tval:45; tchar: '='; flags: $00000000; p1: 0; cost: 10000;
					subval: 23; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 83}
			(name: '& Ring of Protection [%P4]';
					tval:45; tchar: '='; flags: $00000000; p1: 0; cost: 10000;
					subval: 24; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 84}
			(name: '& Ring of See Invisible';
					tval:45; tchar: '='; flags: $01000000; p1: 0; cost: 34000;
					subval: 26; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 85}
			(name: '& Ring of Slaying';
					tval:45; tchar: '='; flags: $00000000; p1: 6; cost: 100000;
					subval: 33; weight: 2; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 86}
			(name: '& Amulet of Wisdom (%P1)';
					tval:40; tchar: '"'; flags: $00000010; p1: 0; cost: 30000;
					subval: 1; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 87}
			(name: '& Amulet of Charisma (%P1)';
					tval:40; tchar: '"'; flags: $00000020; p1: 0; cost: 25000;
					subval: 2; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 88}
			(name: '& Amulet of Searching (%P1)';
					tval:40; tchar: '"'; flags: $00000040; p1: 0; cost: 25000;
					subval: 3; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 89}
			(name: '& Amulet of Slow Digestion';
					tval:40; tchar: '"'; flags: $00000080; p1: 0; cost: 20000;
					subval: 4; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 90}
			(name: '& Amulet of the Magi';
					tval:40; tchar: '"'; flags: $01800040; p1: 0; cost: 500000;
					subval: 5; weight: 3; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 3; damage: '0d0'; level: 50), { 91}
			(name: '& Scroll~ of Enchant Weapon To-Hit';
					tval:70; tchar: '?'; flags: $00000001; p1: 0; cost: 12500;
					subval: 257; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 92}
			(name: '& Scroll~ of Enchant Weapon To-Dam';
					tval:70; tchar: '?'; flags: $00000002; p1: 0; cost: 12500;
					subval: 258; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 93}
			(name: '& Scroll~ of Enchant Armor';
					tval:70; tchar: '?'; flags: $00000004; p1: 0; cost: 12500;
					subval: 259; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 94}
			(name: '& Scroll~ of Identify';
					tval:70; tchar: '?'; flags: $00000008; p1: 0; cost: 5000;
					subval: 260; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 95}
			(name: '& Scroll~ of Teleport';
					tval:70; tchar: '?'; flags: $00000100; p1: 0; cost: 4000;
					subval: 265; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 96}
			(name: '& Scroll~ of Teleportation';
					tval:70; tchar: '?'; flags: $00000200; p1: 0; cost: 9000;
					subval: 266; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 97}
			(name: '& Scroll~ of Rune of Protection';
					tval:70; tchar: '?'; flags: $00002000; p1: 0; cost: 50000;
					subval: 270; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 98}
			(name: '& Scroll~ of Door/Stair Location';
					tval:70; tchar: '?'; flags: $00020000; p1: 0; cost: 3500;
					subval: 274; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), { 99}
			(name: '& Scroll~ of Mass Genocide';
					tval:70; tchar: '?'; flags: $00040000; p1: 0; cost: 100000;
					subval: 275; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), {100}
			(name: '& Scroll~ of Detect Invisible';
					tval:70; tchar: '?'; flags: $00080000; p1: 0; cost: 1500;
					subval: 276; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), {101}
			(name: '& Scroll~ of Recharging';
					tval:70; tchar: '?'; flags: $01000000; p1: 0; cost: 2000;
					subval: 281; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), {102}
			(name: '& Scroll~ of Genocide';
					tval:70; tchar: '?'; flags: $02000000; p1: 0; cost: 75000;
					subval: 282; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), {103}
			(name: '& Scroll~ of Protection from Evil';
					tval:70; tchar: '?'; flags: $08000000; p1: 0; cost: 5000;
					subval: 284; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), {104}
			(name: '& Scroll~ of Dispell Undead';
					tval:70; tchar: '?'; flags: $20000000; p1: 0; cost: 20000;
					subval: 286; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), {105}
			(name: '& Scroll~ of *Enchant Weapon*';
					tval:71; tchar: '?'; flags: $00000001; p1: 0; cost: 50000;
					subval: 257; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), {106}
			(name: '& Scroll~ of *Enchant Armor*';
					tval:71; tchar: '?'; flags: $00000004; p1: 0; cost: 50000;
					subval: 259; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), {107}
			(name: '& Scroll~ of Word-of-Recall';
					tval:71; tchar: '?'; flags: $00000100; p1: 0; cost: 15000;
					subval: 265; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), {108}
			(name: '& Scroll~ of *Destruction*';
					tval:71; tchar: '?'; flags: $00000200; p1: 0; cost: 25000;
					subval: 266; weight: 5; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '0d0'; level: 50), {109}
			(name: '& Potion~ of Gain Strength';
					tval:75; tchar: '!'; flags: $00000001; p1: 0; cost: 20000;
					subval: 257; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {110}
			(name: '& Potion~ of Restore Strength';
					tval:75; tchar: '!'; flags: $00000004; p1: 0; cost: 30000;
					subval: 259; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {111}
			(name: '& Potion~ of Gain Intelligence';
					tval:75; tchar: '!'; flags: $00000008; p1: 0; cost: 30000;
					subval: 260; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {112}
			(name: '& Potion~ of Restore Intelligence';
					tval:75; tchar: '!'; flags: $00000020; p1: 0; cost: 30000;
					subval: 262; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {113}
			(name: '& Potion~ of Gain Wisdom';
					tval:75; tchar: '!'; flags: $00000040; p1: 0; cost: 30000;
					subval: 263; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {114}
			(name: '& Potion~ of Restore Wisdom';
					tval:75; tchar: '!'; flags: $00000100; p1: 0; cost: 30000;
					subval: 265; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {115}
			(name: '& Potion~ of Charisma';
					tval:75; tchar: '!'; flags: $00000200; p1: 0; cost: 30000;
					subval: 266; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {116}
			(name: '& Potion~ of Restore Charisma';
					tval:75; tchar: '!'; flags: $00000800; p1: 0; cost: 30000;
					subval: 268; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {117}
			(name: '& Potion~ of Gain Constitution';
					tval:75; tchar: '!'; flags: $00010000; p1: 0; cost: 30000;
					subval: 273; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {118}
			(name: '& Potion~ of Gain Experience';
					tval:75; tchar: '!'; flags: $00020000; p1: 0; cost: 250000;
					subval: 274; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {119}
			(name: '& Potion~ of Haste Self';
					tval:75; tchar: '!'; flags: $00400000; p1: 0; cost: 7500;
					subval: 279; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {120}
			(name: '& Potion~ of Gain Dexterity';
					tval:75; tchar: '!'; flags: $02000000; p1: 0; cost: 30000;
					subval: 284; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {121}
			(name: '& Potion~ of Restore Dexterity';
					tval:75; tchar: '!'; flags: $04000000; p1: 0; cost: 30000;
					subval: 285; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {122}
			(name: '& Potion~ of Restore Constitution';
					tval:75; tchar: '!'; flags: $68000000; p1: 0; cost: 30000;
					subval: 286; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {123}
			(name: '& Potion~ of Learning';
					tval:76; tchar: '!'; flags: $00000001; p1: 0; cost: 20000;
					subval: 287; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {124}
			(name: '& Potion~ of Invulnerability';
					tval:76; tchar: '!'; flags: $00000008; p1: 0; cost: 25000;
					subval: 290; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {125}
			(name: '& Potion~ of Restore Life Levels';
					tval:76; tchar: '!'; flags: $00000080; p1: 0; cost: 40000;
					subval: 294; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {126}
			(name: '& Potion~ of Detect Invisible';
					tval:76; tchar: '!'; flags: $00000400; p1: 0; cost: 5000;
					subval: 297; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {127}
			(name: '& Potion~ of Neutralize Poison';
					tval:76; tchar: '!'; flags: $00001000; p1: 0; cost: 7500;
					subval: 299; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {128}
			(name: '& Potion~ of Restore Mana';
					tval:76; tchar: '!'; flags: $00002000; p1: 0; cost: 3500;
					subval: 300; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {129}
			(name: '& Potion~ Infra-Vision';
					tval:76; tchar: '!'; flags: $00004000; p1: 0; cost: 2000;
					subval: 301; weight: 4; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {130}
			(name: '& Flask~ of oil';
					tval:77; tchar: '!'; flags: $00040000; p1: 7500; cost: 300;
					subval: 257; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '2d6'; level: 50), {131}
			(name: '& Wand of Light (%P1 charges)';
					tval:65; tchar: '-'; flags: $00000001; p1: 0; cost: 20000;
					subval: 1; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {132}
			(name: '& Wand of Lightning Bolts (%P1 charges)';
					tval:65; tchar: '-'; flags: $00000002; p1: 0; cost: 60000;
					subval: 2; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {133}
			(name: '& Wand of Frost Bolts (%P1 charges)';
					tval:65; tchar: '-'; flags: $00000004; p1: 0; cost: 80000;
					subval: 3; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {134}
			(name: '& Wand of Fire Bolts (%P1 charges)';
					tval:65; tchar: '-'; flags: $00000008; p1: 0; cost: 100000;
					subval: 4; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {135}
			(name: '& Wand of Stone-to-Mud (%P1 charges)';
					tval:65; tchar: '-'; flags: $00000010; p1: 0; cost: 30000;
					subval: 5; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {136}
			(name: '& Wand of Polymorph (%P1 charges)';
					tval:65; tchar: '-'; flags: $00000020; p1: 0; cost: 40000;
					subval: 6; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {137}
			(name: '& Wand of Slow Monster (%P1 charges)';
					tval:65; tchar: '-'; flags: $00000100; p1: 0; cost: 50000;
					subval: 9; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {138}
			(name: '& Wand of Confuse Monster (%P1 charges)';
					tval:65; tchar: '-'; flags: $00000200; p1: 0; cost: 40000;
					subval: 10; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {139}
			(name: '& Wand of Sleep Monster (%P1 charges)';
					tval:65; tchar: '-'; flags: $00000400; p1: 0; cost: 50000;
					subval: 11; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {140}
			(name: '& Wand of Drain Life (%P1 charges)';
					tval:65; tchar: '-'; flags: $00000800; p1: 0; cost: 120000;
					subval: 12; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {141}
			(name: '& Wand of Trap/Door destruction (%P1 charges)';
					tval:65; tchar: '-'; flags: $00001000; p1: 0; cost: 10000;
					subval: 13; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {142}
			(name: '& Wand of Magic Missle (%P1 charges)';
					tval:65; tchar: '-'; flags: $00002000; p1: 0; cost: 20000;
					subval: 14; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {143}
			(name: '& Wand of Wall Building (%P1 charges)';
					tval:65; tchar: '-'; flags: $00004000; p1: 0; cost: 40000;
					subval: 15; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {144}
			(name: '& Wand of Clone Monster (%P1 charges)';
					tval:65; tchar: '-'; flags: $00008000; p1: 0; cost: 15000;
					subval: 16; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {145}
			(name: '& Wand of Teleport Away (%P1 charges)';
					tval:65; tchar: '-'; flags: $00010000; p1: 0; cost: 35000;
					subval: 17; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {146}
			(name: '& Wand of Disarming (%P1 charges)';
					tval:65; tchar: '-'; flags: $00020000; p1: 0; cost: 70000;
					subval: 18; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {147}
			(name: '& Wand of Lightning Balls (%P1 charges)';
					tval:65; tchar: '-'; flags: $00040000; p1: 0; cost: 120000;
					subval: 19; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {148}
			(name: '& Wand of Cold Balls (%P1 charges)';
					tval:65; tchar: '-'; flags: $00080000; p1: 0; cost: 150000;
					subval: 20; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {149}
			(name: '& Wand of Fire Balls (%P1 charges)';
					tval:65; tchar: '-'; flags: $00100000; p1: 0; cost: 180000;
					subval: 21; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {150}
			(name: '& Wand of Stinking Cloud (%P1 charges)';
					tval:65; tchar: '-'; flags: $00200000; p1: 0; cost: 40000;
					subval: 21; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {151}
			(name: '& Wand of Acid Balls (%P1 charges)';
					tval:65; tchar: '-'; flags: $00400000; p1: 0; cost: 165000;
					subval: 21; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {152}
			(name: '& Wand of Wonder (%P1 charges)';
					tval:65; tchar: '-'; flags: $00800000; p1: 0; cost: 25000;
					subval: 22; weight: 10; number: 1; tohit: 0; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {153}
			(name: '& Staff of Light (%P1 charges)';
					tval:55; tchar: '_'; flags: $00000001; p1: 0; cost:
					25000; subval: 1; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {154}
			(name: '& Staff of Door/Stair Location (%P1 charges)';
					tval:55; tchar: '_'; flags: $00000002; p1: 0; cost:
					35000; subval: 2; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {155}
			(name: '& Staff of Trap Location (%P1 charges)';
					tval:55; tchar: '_'; flags: $00000004; p1: 0; cost:
					35000; subval: 3; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {156}
			(name: '& Staff of Treasure Location (%P1 charges)';
					tval:55; tchar: '_'; flags: $00000008; p1: 0; cost:
					20000; subval: 4; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {157}
			(name: '& Staff of Object Location (%P1 charges)';
					tval:55; tchar: '_'; flags: $00000010; p1: 0; cost:
					20000; subval: 5; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {158}
			(name: '& Staff of Teleportation (%P1 charges)';
					tval:55; tchar: '_'; flags: $00000020; p1: 0; cost:
					40000; subval: 6; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {159}
			(name: '& Staff of Earthquakes (%P1 charges)';
					tval:55; tchar: '_'; flags: $00000040; p1: 0; cost:
					35000; subval: 7; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {160}
			(name: '& Staff of Summoning (%P1 charges)';
					tval:55; tchar: '_'; flags: $00000080; p1: 0; cost:
					20000; subval: 8; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {161}
			(name: '& Staff of *Destruction* (%P1 charges)';
					tval:55; tchar: '_'; flags: $00000200; p1: 0; cost:
					250000; subval: 10; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {162}
			(name: '& Staff of Starlite (%P1 charges)';
					tval:55; tchar: '_'; flags: $00000400; p1: 0; cost:
					80000; subval: 11; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {163}
			(name: '& Staff of Slow Monsters (%P1 charges)';
					tval:55; tchar: '_'; flags: $00001000; p1: 0; cost:
					80000; subval: 13; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {164}
			(name: '& Staff of Sleep Monsters (%P1 charges)';
					tval:55; tchar: '_'; flags: $00002000; p1: 0; cost:
					70000; subval: 14; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {165}
			(name: '& Staff of Detect Invisible (%P1 charges)';
					tval:55; tchar: '_'; flags: $00008000; p1: 0; cost:
					20000; subval: 16; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {166}
			(name: '& Staff of Speed (%P1 charges)';
					tval:55; tchar: '_'; flags: $00010000; p1: 0; cost:
					80000; subval: 17; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {167}
			(name: '& Staff of Mass Polymorph (%P1 charges)';
					tval:55; tchar: '_'; flags: $00040000; p1: 0; cost:
					75000; subval: 19; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {168}
			(name: '& Staff of Remove Curse (%P1 charges)';
					tval:55; tchar: '_'; flags: $00080000; p1: 0; cost:
					50000; subval: 20; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {169}
			(name: '& Staff of Detect Evil (%P1 charges)';
					tval:55; tchar: '_'; flags: $00100000; p1: 0; cost:
					35000; subval: 21; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {170}
			(name: '& Staff of Curing (%P1 charges)';
					tval:55; tchar: '_'; flags: $00200000; p1: 0; cost:
					100000; subval: 22; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {171}
			(name: '& Staff of Dispell Evil (%P1 charges)';
					tval:55; tchar: '_'; flags: $00400000; p1: 0; cost:
					120000; subval: 23; weight: 50; number: 1; tohit: 0; todam:
					0; ac: 0; toac: 0; damage: '1d2'; level: 50), {172}
			(name: '& Magic Book [Beginners-Magik+Magik I]';
					tval:90; tchar: '?'; flags: $0000FFFF; p1: 0; cost: 40000;
					subval: 257; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {173}
			(name: '& Magic Book [Magik II+Mages Guide to Power]';
					tval:90; tchar: '?'; flags: $7FFF0000; p1: 0; cost: 80000;
					subval: 259; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {174}
			(name: '& Prayer Book [Beginners Handbook+Words of Wisdom]';
					tval:91; tchar: '?'; flags: $0000FFFF; p1: 0; cost: 2500;
					subval: 258; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50), {175}
			(name: '& Prayer Book [Chants+Blessings+Exorcism+Dispelling]';
					tval:91; tchar: '?'; flags: $7FFF0000; p1: 0; cost: 90000;
					subval: 261; weight: 30; number: 1; tohit: -100; todam: 0;
					ac: 0; toac: 0; damage: '1d1'; level: 50));{176}



{ Treasure related values }
	inven_ctr : {[psect(player$data)]} integer = 0;  { Total different obj's }
	inven_weight : {[psect(player$data)]} integer = 0;  { Cur carried weight    }
//	equip_ctr : {[psect(player$data)]} integer = 0;  { Cur equipment ctr }
	tcptr : {[psect(moria$data)]} integer;  { Cur treasure heap ptr }


{ Following are creature arrays and variables   }
	c_list : {[psect(moria$data)]} array [1..400] of creature_type;
	m_list : {[psect(moria$data)]} array [1..MAX_MALLOC] of monster_type;
	m_level : {[psect(generate$data)]} array [0..MAX_MONS_LEVEL] of integer;

	muptr : {[psect(generate$data)]} integer;  { Cur used monster ptr  }
	mfptr : {[psect(generate$data)]} integer;  { Cur free monster ptr  }
	mon_tot_mult : {[psect(moria$data)]} integer;  { # of repro's of creature  }

	{ Following are arrays for descriptive pieces   }
	colors : {[psect(setup$data)]} array [1..MAX_COLORS] of atype = (
			'Amber','Azure','Blue','Blue Speckled','Blue Spotted',
			'Black','Black Speckled','Black Spotted','Brown',
			'Brown Speckled','Brown Spotted','Bubbling',
			'Chartreuse','Clear','Cloudy','Copper','Copper Spotted',
			'Crimson','Cyan','Dark Blue','Dark Green','Dark Red',
			'Ecru','Gold','Gold Spotted','Green','Green Speckled',
			'Green Spotted','Grey','Grey Spotted','Hazy','Indigo',
			'Light Blue','Light Green','Magenta','Metallic Blue',
			'Metallic Red','Metallic Green','Metallic Purple',
			'Misty','Orange','Orange Speckled','Orange Spotted',
			'Pink','Pink Speckled','Plaid','Puce','Purple',
			'Purple Speckled','Purple Spotted','Red','Red Speckled',
			'Red Spotted','Silver','Silver Speckled',
			'Silver Spotted','Smokey','Tan','Tangerine','Topaz',
			'Turquoise','Violet','Vermilion','White',
			'White Speckled','White Spotted','Yellow' );

	mushrooms : {[psect(setup$data)]} array [1..MAX_MUSH] of atype = (
			'Blue','Black','Brown','Copper','Crimson','Dark blue',
			'Dark green','Dark red','Gold','Green','Grey',
			'Light Blue','Light Green','Orange','Pink','Plaid',
			'Purple','Red','Tan','Turquoise','Violet','White',
			'Yellow','Wrinkled','Wooden','Slimey','Speckled',
			'Spotted','Furry' );

	woods : {[psect(setup$data)]} array [1..MAX_WOODS] of atype = (
			'Applewood','Ashen','Aspen','Avocado wood','Balsa',
			'Banyan','Birch','Ceder','Cherrywood','Cinnibar',
			'Cottonwood','Cypress','Dogwood','Driftwood','Ebony',
			'Elm wood','Eucalyptus','Grapevine','Hawthorn','Hemlock',
			'Hickory','Iron wood','Juniper','Locust','Mahogany',
			'Magnolia','Manzanita','Maple','Mulberry','Oak','Pecan',
			'Persimmon','Pine','Redwood','Rosewood','Spruce','Sumac',
			'Sycamore','Teak','Walnut','Zebra wood' );

	metals : {[psect(setup$data)]} array [1..MAX_METALS] of atype = (
			'Aluminium','Bone','Brass','Bronze','Cast Iron',
			'Chromium','Copper','Gold','Iron','Lead','Magnesium',
			'Molybdenum','Nickel','Pewter','Rusty','Silver','Steel',
			'Tin','Titanium','Tungsten','Zirconium','Zinc',
			'Aluminium Plated','Brass Plated','Copper Plated',
			'Gold Plated','Nickel Plated','Silver Plated',
			'Steel Plated','Tin Plated','Zinc Plated' );

	rocks : {[psect(setup$data)]} array [1..MAX_ROCKS] of atype = (
			'Amber','Agate','Alexandrite','Amethyst','Antlerite',
			'Aquamarine','Argentite','Azurite','Beryl','Bloodstone',
			'Calcite','Carnelian','Coral','Corundum','Cryolite',
			'Diamond','Diorite','Emerald','Flint','Fluorite',
			'Gabbro','Garnet','Granite','Gypsum','Hematite','Jade',
			'Jasper','Kryptonite','Lapus lazuli','Limestone',
			'Malachite','Manganite','Marble','Moonstone',
			'Neptunite','Obsidian','Onyx','Opal','Pearl','Pyrite',
			'Quartz','Quartzite','Rhodonite','Rhyolite','Ruby',
			'Saphire','Sphalerite','Staurolite','Tiger eye','Topaz',
			'Turquoise','Zircon' );

	amulets : {[psect(setup$data)]} array [1..MAX_AMULETS] of atype = (
			'Birch','Ceder','Dogwood','Driftwood','Elm wood',
			'Hemlock','Hickory','Mahogany','Maple','Oak','Pine',
			'Redwood','Rosewood','Walnut','Aluminium','Bone','Brass',
			'Bronze','Copper','Iron','Lead','Nickel','Agate',
			'Amethyst','Diamond','Emerald','Flint','Garnet','Jade',
			'Obsidian','Onyx','Opal','Pearl','Quartz','Ruby',
			'Saphire','Tiger eye','Topaz','Turquoise' );


	{ Following are variables for the Save Character Routines   }
	finam : {[psect(save$data)]} vtype;
//	data_corrupt: {[psect(save$data)]} boolean;
	key_rec : {[psect(save$data)]} key_type;
	inSave: Boolean = False;

{ Creatures must be defined here. See TYPES.INC under creature_type for a
  complete list of all variables for creatures.  Some of the less obvious
  are explained below.

		Hit points:     '#1d#2' where #2 is the range of each roll and
						#1 is the number of added up rolls to make.
						Example: a creature with 5 eight-sided hit die
						is given '5d8'.

				Attack types:
				1       Normal attack
				2       Poison Strength
				3       Confusion attack
				4       Fear attack
				5       Fire attack
				6       Acid attack
				7       Cold attack
				8       Lightning attack
				9       Corrosion attack
				10      Blindness attack
				11      Paralysis attack
				12      Steal Money
				13      Steal Object
				14      Poison
				15      Lose dexterity
				16      Lose constitution
				17      Lose intelligence
				18      Lose wisdom
				19      Lose experience
				20      Aggravation
				21      Disenchants
				22      Eats food
				23      Eats light
				24      Eats charges
				25      Turn to stone
				26      Vampire Bite
				27	    Mind Flayer Attack
				28	    Hell Hose Attack
				29	    Vargouille Attack
				99      Blank

				Attack descriptions:
				1       hits you.
				2       bites you.
				3       claws you.
				4       stings you.
				5       touches you.
				6       kicks you.
				7       gazes at you.
				8       breathes on you.
				9       spits on you.
				10      makes a horrible wail.
				11      embraces you.
				12      crawls on you.
				13      releases a cloud of spores.
				14      begs you for money.
				15      You've been slimed.
				16      crushes you.
				17      tramples you.
				18      drools on you.
				20	    touches you with its tentacles.
				99      is repelled.

		Example:  For a creature which bites for 1d6,then stings for
				  2d4 and loss of dex you would use:
						'1 2 1d6|15 4 2d4'

				CMOVE flags:
Movement.       00000001        Move only to attack
		.       00000002        Move,attack normal
		.       00000008        20% random movement
		.       00000010        40% random movement
		.       00000020        75% random movement
Special +       00010000        Invisible movement
		+       00020000        Move through door
		+       00040000        Move through wall
		+       00080000        Move through creatures
		+       00100000        Picks up objects
		+       00200000        Multiply monster
Carries =       01000000        Carries objects.
		=       02000000        Carries gold.
		=       04000000        Has 60% of time.
		=       08000000        Has 90% of time.
		=       10000000        1d2 objects/gold.
		=       20000000        2d2 objects/gold.
		=       40000000        4d2 objects/gold.
Special ~       80000000        Win-the-Game creature.

				SPELL Flags:
Frequency       000001    1     These add up to x.  Then
(1 in x).       000002    2     if RANDINT(X) = 1 the
		.       000004    4     creature casts a spell.
		.       000008    8
Spells  =       000010  Teleport short (blink)
		=       000020  Teleport long
		=       000040  Teleport player to monster
		=       000080  Cause light wound
		=       000100  Cause serious wound
		=       000200  Hold person (Paralysis)
		=       000400  Cause blindness
		=       000800  Cause confusion
		=       001000  Cause fear
		=       002000  Summon monster
		=       004000  Summon undead
		=       008000  Slow Person
		=       010000  Drain Mana
		=       020000  Not Used
		=       040000  Not Used
Breaths +       080000  Breathe Lightning
		+       100000  Breathe Gas
		+       200000  Breathe Acid
		+       400000  Breathe Frost
		+       800000  Breathe Fire

				CDEFENSE flags:
				0001    Hurt by Slay Dragon.
				0002    Hurt by Slay Monster.
				0004    Hurt by Slay Evil.
				0008    Hurt by Slay Undead.
				0010    Hurt by Frost.
				0020    Hurt by Fire.
				0040    Hurt by Poison.
				0080    Hurt by Acid.
				0100    Hurt by Light-Wand.
				0200    Hurt by Stone-to-Mud.
				0400    Minus experience.
				0800    Not used.
				1000    Cannot be charmed or slept.
				2000    Can be seen with infra-vision.
				4000    Max Hit points.
				8000    Not used.


		Sleep (sleep)   :       A measure in turns of how fast creature
								will notice player (on the average).
		Area of affect (aaf) :  Max range that creature is able to "notice"
								the player.

 Creature data goes here - mwk,opusii

		 c_list := (
('Filthy Street Urchin',%X'0012000A',%X'00000000',%X'2034',400,0,
   4,1,1,'p','1d4','1 14 0d0|12 5 0d0' ,0),etc.... );

		blank_monster   := (0,0,0,0,0,0,0,0,0,false,false);}

