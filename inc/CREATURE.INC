{ Creatures movement and attacking are done from here }
procedure creatures(attack: boolean);
	var
	i1, i2, i3, moldy, moldx, tmp: integer;
	tmp_colr: AnsiChar;

 { Updates screen when monsters move about }
	procedure update_mon(monptr: integer);
		var
		flag: boolean;

		begin
		with m_list[monptr] do
			with cave[fy, fx] do
				begin
				flag:= false;
				if (cdis <= max_sight) then
					if (py.flags.blind < 1) then
						if (panel_contains(fy, fx)) then
							if (los(char_row, char_col, fy, fx)) then
								with c_list[mptr] do
									begin
{ Normal sight. }
									if ((pl) or (tl)) then
										begin
										if (py.flags.see_inv) then
											flag:= true
										else if (($10000 and cmove) = 0) then
											flag:= true
										end
{ Infra vision. }
									else if (py.flags.see_infra > 0) then
										if (cdis <= py.flags.see_infra) then
											if (($2000 and cdefense) <> 0) then
												flag:= true
									end;
{ Light it up. }
				if (flag) then
					begin
//dengland 			Always do monster appear code when not attacking
					if  (not attack)
					or  (not(ml)) then
						begin
						get_mon_background(m_list[monptr], tmp_colr);
						get_mon_foreground(m_list[monptr], tmp_colr);

						print(c_list[mptr].cchar, tmp_colr, fy, fx);

//						first check ml
						if not ml then
							begin
							if (search_flag) then
								search_off;
							if (py.flags.rest > 0) then
								rest_off;

							if (find_flag) then
								begin
								find_flag:= false;
								move_char(5)
								end
							end;

						flush;
						ml:= True;
						end
					end
{ Turn it off. }
				else if (ml) then
					begin
					ml:= false;
					if ((tl) or (pl)) then
						lite_spot(fy, fx)
					else
						unlite_spot(fy, fx)
					end
				end
		end;

 { Move the critters about the dungeon }
	function mon_move(monptr: integer): boolean;
		type
		mm_type = array [1 .. 5] of integer;

		var
		i1, i2, i3: integer;
		mm:         mm_type;
//		out_val:    vtype;
		move_test:  boolean;

{ Choose correct directions for monster movement }
		procedure get_moves(monptr: integer; var mm: mm_type);
			var
			y, ay, X, ax, move_val: integer;

			begin
			y := m_list[monptr].fy - char_row;
			X := m_list[monptr].fx - char_col;
			ay:= abs(y);
			ax:= abs(X);
			if (y < 0) then
				move_val:= 8
			else
				move_val:= 0;
			if (X > 0) then
				move_val:= move_val + 4;
			if (ay > (ax * 1.7321)) then
				move_val:= move_val + 2
			else if (ax > (ay * 1.7321)) then
				move_val:= move_val + 1;
			case move_val of
				0:
					begin
					mm[1]:= 9;
					if (ay > ax) then
						begin
						mm[2]:= 8;
						mm[3]:= 6;
						mm[4]:= 7;
						mm[5]:= 3
						end
					else
						begin
						mm[2]:= 6;
						mm[3]:= 8;
						mm[4]:= 3;
						mm[5]:= 7
						end
					end;
				1, 9:
					begin
					mm[1]:= 6;
					if (y < 0) then
						begin
						mm[2]:= 3;
						mm[3]:= 9;
						mm[4]:= 2;
						mm[5]:= 8
						end
					else
						begin
						mm[2]:= 9;
						mm[3]:= 3;
						mm[4]:= 8;
						mm[5]:= 2
						end
					end;
				2, 6:
					begin
					mm[1]:= 8;
					if (X < 0) then
						begin
						mm[2]:= 9;
						mm[3]:= 7;
						mm[4]:= 6;
						mm[5]:= 4
						end
					else
						begin
						mm[2]:= 7;
						mm[3]:= 9;
						mm[4]:= 4;
						mm[5]:= 6
						end
					end;
				4:
					begin
					mm[1]:= 7;
					if (ay > ax) then
						begin
						mm[2]:= 8;
						mm[3]:= 4;
						mm[4]:= 9;
						mm[5]:= 1
						end
					else
						begin
						mm[2]:= 4;
						mm[3]:= 8;
						mm[4]:= 1;
						mm[5]:= 9
						end
					end;
				5, 13:
					begin
					mm[1]:= 4;
					if (y < 0) then
						begin
						mm[2]:= 1;
						mm[3]:= 7;
						mm[4]:= 8;
						mm[5]:= 2
						end
					else
						begin
						mm[2]:= 7;
						mm[3]:= 1;
						mm[4]:= 2;
						mm[5]:= 8
						end
					end;
				8:
					begin
					mm[1]:= 3;
					if (ay > ax) then
						begin
						mm[2]:= 2;
						mm[3]:= 6;
						mm[4]:= 1;
						mm[5]:= 9
						end
					else
						begin
						mm[2]:= 6;
						mm[3]:= 2;
						mm[4]:= 9;
						mm[5]:= 1
						end
					end;
				10, 14:
					begin
					mm[1]:= 2;
					if (X < 0) then
						begin
						mm[2]:= 1;
						mm[3]:= 3;
						mm[4]:= 4;
						mm[5]:= 6
						end
					else
						begin
						mm[2]:= 3;
						mm[3]:= 1;
						mm[4]:= 6;
						mm[5]:= 4
						end
					end;
				12:
					begin
					mm[1]:= 1;
					if (ay > ax) then
						begin
						mm[2]:= 2;
						mm[3]:= 4;
						mm[4]:= 3;
						mm[5]:= 7
						end
					else
						begin
						mm[2]:= 4;
						mm[3]:= 2;
						mm[4]:= 7;
						mm[5]:= 3
						end
					end
				end;
			end;

 { Make an attack on the player (chuckle...) }
		procedure make_attack(monptr: integer);
			var
			xpos,
			atype,
			adesc,
			dam,
			i1: Integer;
//			i2: Integer;
//			i3,
//			i4: integer;
			attstr,
			attx,
			cdesc: vtype;
//			mdesc,
			ddesc: AnsiString;
			damstr: AnsiString;
			flag:   boolean;
			t: TMoriaInvenType;

			begin
			with m_list[monptr] do
				with c_list[mptr] do
					begin
					attstr:= damage;
					if ((($10000 and cmove) <> 0) and
								(not(py.flags.see_inv))) then
						cdesc:= 'It '
					else if (py.flags.blind > 0) then
						cdesc:= 'It '
					else if (not(ml)) then
						cdesc:= 'It '
					else
						cdesc:= 'The ' + name + ' ';
{ For 'DIED_FROM' string }
					if (($80000000 and cmove) <> 0) then
						ddesc:= 'The ' + name
					else
						ddesc                  := '& ' + name;
					invenTemp.name  := ddesc;
					invenTemp.number:= 1;
					objdes(ddesc, @invenTemp, true);
{ End DIED_FROM }
					while (Length(attstr) > 0) do
						begin
						xpos:= index(attstr, '|');
						if (xpos > 0) then
							begin
							attx  := Copy(attstr, 1, xpos - 1);
							attstr:= Copy(attstr, xpos + 1,
									Length(attstr) - xpos)
							end
						else
							begin
							attx  := attstr;
							attstr:= ''
							end;
						sscanf(attx, '%d%d%s', [@atype, @adesc, @damstr]);
						flag:= false;
						if (py.flags.protevil > 0) then
							if ((cdefense and $0004) <> 0) then
								if ((py.misc.lev + 1) > level) then
									begin
									atype:= 99;
									adesc:= 99
									end;
						with py.misc do
							case atype of
{ Normal attack }				1:
									if (test_hit(60, level, 0, pac + ptoac)) then
										flag:= true;
{ Poison Strength }				2:
									if (test_hit(0, level, 0, pac + ptoac)) then
										flag:= true;
{ Confusion attack }			3:
									if (test_hit(10, level, 0, pac + ptoac)) then
										flag:= true;
{ Fear attack }					4:
									if (test_hit(10, level, 0, pac + ptoac)) then
										flag:= true;
{ Fire attack }					5:
									if (test_hit(10, level, 0, pac + ptoac)) then
										flag:= true;
{ Acid attack }					6:
									if (test_hit(0, level, 0, pac + ptoac)) then
										flag:= true;
{ Cold attack }					7:
									if (test_hit(10, level, 0, pac + ptoac)) then
										flag:= true;
{ Lightning attack }			8:
									if (test_hit(10, level, 0, pac + ptoac)) then
										flag:= true;
{ Corrosion attack }			9:
									if (test_hit(0, level, 0, pac + ptoac)) then
										flag:= true;
{ Blindness attack } 			10:
									if (test_hit(0, level, 0, pac + ptoac)) then
										flag:= true;
{ Paralysis attack }			11:
									if (test_hit(4, level, 0, pac + ptoac)) then
										flag:= true;
{ Steal Money }					12:
									if (test_hit(8, level, 0, py.misc.lev)) then
										if (py.misc.au > 0) then
											flag:= true;
{ Steal Object }				13:
									if (test_hit(5, level, 0, py.misc.lev)) then
										if (inven_ctr > 0) then
											flag:= true;
{ Poison }						14:
									if (test_hit(10, level, 0, pac + ptoac)) then
										flag:= true;
{ Lose dexterity }				15:
									if (test_hit(0, level, 0, pac + ptoac)) then
										flag:= true;
{ Lose constitution }			16:
									if (test_hit(0, level, 0, pac + ptoac)) then
										flag:= true;
{ Lose intelligence }			17:
									if (test_hit(0, level, 0, pac + ptoac)) then
										flag:= true;
{ Lose wisdom }					18:
									if (test_hit(0, level, 0, pac + ptoac)) then
										flag:= true;
{ Lose experience }				19:
									if (test_hit(10, level, 0, pac + ptoac)) then
										flag:= true;
{ Aggravate monsters }			20:
									flag:= true;
{ Disenchant }					21:
									if (test_hit(7, level, 0, pac + ptoac)) then
										flag:= true;
{ Eat food }					22:
									if (test_hit(0, level, 0, pac + ptoac)) then
										flag:= true;
{ Eat light }					23:
									if (test_hit(0, level, 0, pac + ptoac)) then
										flag:= true;
{ Eat charges }					24:
									if (test_hit(10, level, 0, pac + ptoac)) then
										flag:= true;
{ Vampire bite }				25:
									if (test_hit(10, level, 0, pac + ptoac)) then
										flag:= true;
{ Turn to stone }				26:
									if (test_hit(10, level, 0, pac + ptoac)) then
										flag:= true;
{ Vargouillian bite }			29:
									if (test_hit(3, level, 0, pac + ptoac)) then
										flag:= true;
								99:
									flag:= true;
								else
								end;
						if (flag) then
							begin
							case adesc of
								1:
									msg_print(cdesc + 'hits you.');
								2:
									msg_print(cdesc + 'bites you.');
								3:
									msg_print(cdesc + 'claws you.');
								4:
									msg_print(cdesc + 'stings you.');
								5:
									msg_print(cdesc + 'touches you.');
								6:
									msg_print(cdesc + 'kicks you.');
								7:
									msg_print(cdesc + 'gazes at you.');
								8:
									msg_print(cdesc + 'breathes on you.');
								9:
									msg_print(cdesc + 'spits on you.');
								10:
									msg_print(cdesc + 'makes a horrible wail.');
								11:
									msg_print(cdesc + 'embraces you.');
								12:
									msg_print(cdesc + 'crawls on you.');
								13:
									msg_print(cdesc + 'releases a cloud of spores.');
								14:
									msg_print(cdesc + 'begs you for money.');
								15:
									msg_print('You''ve been slimed!');
								16:
									msg_print(cdesc + 'crushes you.');
								17:
									msg_print(cdesc + 'tramples you.');
								18:
									msg_print(cdesc + 'drools on you.');
{ Iggy attacks }				19:
									begin
									if (py.flags.see_inv) then
										REB_Iggy:= 11
									else
										REB_Iggy:= 7;
									case randint(REB_Iggy) of
										1:
											msg_print(cdesc + 'insults you.');
										2:
											msg_print(cdesc + 'insults your mother.');
										3:
											msg_print(cdesc + 'wets on your leg.');
										4:
											msg_print(cdesc + 'defiles you.');
										5:
											msg_print(cdesc + 'humiliates you.');
										6:
											msg_print(cdesc + 'laughs at you.');
										7:
											msg_print(cdesc + 'curses you!');
										8:
											msg_print(cdesc + 'moons you!');
										9:
											msg_print(cdesc + 'gives you the finger.');
										10:
											msg_print(cdesc + 'makes obscene gestures.');
										11:
											msg_print(cdesc + 'dances around you.');
										end
									end;
								20:
									msg_print(cdesc + 'touches you with its tentacles.');
								99:
									msg_print(cdesc + 'is repelled.');
								else
								end;
							case atype of
{ Normal attack }				1:
									begin
									dam:= damroll(damstr);
									with py.misc do
										dam:= dam - round(((pac + ptoac) / 200.0) * dam);
									take_hit(dam, ddesc);
									prt_chp
									end;
{ Poison Strength }				2:
									begin
									take_hit(damroll(damstr), ddesc);
									if (py.flags.sustain_str) then
										msg_print('You feel weaker for a moment, then it passes.')
									else if (randint(2) = 1) then
										begin
										msg_print('You Feel Weaker.');
										py.stat.cstr:= de_statp(py.stat.cstr);
										prt_strength;
										prt_max_weight;
										end;
									prt_chp
									end;
{ Confusion attack }			3:
									with py.flags do
										begin
										take_hit(damroll(damstr), ddesc);
										if (randint(2) = 1) then
											begin
											if (confused < 1) then
												begin
												msg_print('You feel confused.');
												confused:= confused + randint(level)
												end;
											confused:= confused + 3
											end;
										prt_chp
										end;
{ Fear attack }					4:
									with py.flags do
										begin
										take_hit(damroll(damstr), ddesc);
										if (player_saves(wis_adj)) then
											msg_print('You resist the effects!')
										else if (afraid < 1) then
											begin
											msg_print('You are suddenly afraid!');
											afraid:= afraid + 3 + randint(level)
											end
										else
											afraid:= afraid + 3;
										prt_chp
										end;
{ Fire attack }					5:
									begin
									msg_print('You are enveloped in flames!');
									fire_dam(damroll(damstr), ddesc)
									end;
{ Acid attack }					6:
									begin
									msg_print('You are covered in acid!');
									acid_dam(damroll(damstr), ddesc)
									end;
{ Cold attack }					7:
									begin
									msg_print('You are covered with frost!');
									cold_dam(damroll(damstr), ddesc)
									end;
{ Lightning attack }			8:
									begin
									msg_print('Lightning strikes you!');
									light_dam(damroll(damstr), ddesc)
									end;
{ Corrosion attack }			9:
									begin
									msg_print('A stinging red gas swirls about you.');
									corrode_gas(ddesc);
									take_hit(damroll(damstr), ddesc);
									prt_chp
									end;
{ Blindness attack }			10:
									with py.flags do
										begin
										take_hit(damroll(damstr), ddesc);
										if (blind < 1) then
											begin
											blind:= blind + 10 + randint(level);
											msg_print('Your eyes begin to sting.');
											msg_print(' ')
											end;
										blind:= blind + 5;
										prt_chp
										end;
{ Paralysis attack }			11:
									with py.flags do
										begin
										take_hit(damroll(damstr), ddesc);
										if (player_saves(con_adj)) then
											msg_print('You resist the effects!')
										else if (paralysis < 1) then
											begin
											if (free_act) then
												msg_print('You are unaffected.')
											else
												begin
												paralysis:= randint(level) + 3;
												msg_print('You are paralyzed.')
												end
											end;
										prt_chp
										end;
{ Steal Money }					12:
									with py.misc do
										begin
//dengland								This should be based on monster capability
										if  (randint(124) < py.stat.cdex)
										and (py.flags.paralysis < 1) then
											msg_print('You quickly protect your money pouch!')
										else
											begin
											i1:= (au div  10) + randint(25);
											if (i1 > au) then
												begin
												i1:= au;
												au:= 0;
												end
											else
												au:= au - i1;

											Inc(stoleau, i1);

											msg_print('Your purse feels lighter.');
											prt_gold;
											end;

										if (randint(2) = 1) then
											begin
											msg_print('There is a puff of smoke!');
											teleport_away(monptr, max_sight)
											end
										end;
{ Steal Object }				13:
									with py.stat do
										begin
//dengland								This should be based on monster capability
										if  (randint(124) < py.stat.cdex)
										and (py.flags.paralysis < 1) then
											msg_print('You grab hold of your backpack!')
										else
											begin
											i1:= randint(inven_ctr);
											InvenAbsIdxToTypeIdx(i1, t, i1);
											InvenDestroy(t, i1);
											msg_print('Your backpack feels lighter.')
											end;

										if (randint(2) = 1) then
											begin
											msg_print('There is a puff of smoke!');
											teleport_away(monptr, max_sight)
											end
										end;
{ Poison }						14:
									with py.flags do
										begin
										take_hit(damroll(damstr), ddesc);
										prt_chp;
										msg_print('You feel very sick.');
										poisoned:= poisoned + randint(level) + 5;
										Opusii_vomit(randint(level) + 5);
										end;
{ Lose dexterity }				15:
									with py.flags do
										begin
										take_hit(damroll(damstr), ddesc);
										if (sustain_dex) then
											msg_print('You feel clumsy for a moment, then it passes.')
										else
											begin
											msg_print('You feel more clumsy.');
											py.stat.cdex:= de_statp(py.stat.cdex);
											prt_dexterity
											end;
										prt_chp
										end;
{ Lose constitution }			16:
									with py.flags do
										begin
										take_hit(damroll(damstr), ddesc);
										if (sustain_con) then
											msg_print('Your body resists the effects of the disease.')
										else
											begin
											msg_print('Your health is damaged!');
											py.stat.ccon:= de_statp(py.stat.ccon);
											prt_constitution
											end;
										prt_chp
										end;
{ Lose intelligence }			17:
									with py.flags do
										begin
										take_hit(damroll(damstr), ddesc);
										if (sustain_int) then
											begin
											msg_print('You feel your memories fading...');
											msg_print('Your memories are suddenly restored!')
											end
										else
											begin
											msg_print('You feel your memories fading...');
											py.stat.cint:= de_statp(py.stat.cint);
											prt_intelligence
											end;
										prt_chp;
										end;
{ Lose wisdom }					18:
									with py.flags do
										begin
										take_hit(damroll(damstr), ddesc);
										if (sustain_wis) then
											msg_print('Your wisdom is sustained.')
										else
											begin
											msg_print('Your wisdom is drained.');
											py.stat.cwis:= de_statp(py.stat.cwis);
											prt_wisdom
											end;
										prt_chp
										end;
{ Lose experience }				19:
									begin
									msg_print(
										'You feel your life draining away!');
									i1:= damroll(damstr) +
											(py.misc.exp div 100) * mon_drain_life;
									lose_exp(i1)
									end;
{ Aggravate monster }			20:
									aggravate_monster(5);
{ Disenchant }					21:
									begin
									flag:= false;
									case randint(6) of
										1:
											i1:= Ord(miwMainWeapon);
										2:
											i1:= Ord(miwArmour);
										3:
											i1:= Ord(miwBracers);
										4:
											i1:= Ord(miwCloak);
										5:
											i1:= Ord(miwGloves);
										6:
											i1:= Ord(miwHelmet)
										end;
									with newInventory[mitWornWielding, i1] do
										begin
										if (tohit > 0) then
											begin
											tohit:= tohit - randint(2);
											flag := true
											end;
										if (todam > 0) then
											begin
											todam:= todam - randint(2);
											flag := true
											end;
										if (toac > 0) then
											begin
											toac:= toac - randint(2);
											flag:= true
											end
										end;
									if (flag) then
										begin
										msg_print(
											'There is a static feeling in the air...');
//										py_bonuses(blank_treasure, 1)
										ApplyBonuses(@blank_treasure, 1)
										end
									end;
{ Eat food }					22:
									begin
//									if (find_range([80], i1, i2)) then
//										inven_destroy(i1)
									if FindFirstItem([80], t, i1) then
										InvenDestroy(t, i1);
									end;
{ Eat light }					23:
									begin
									with newInventory[mitWornWielding,
											Ord(miwLight)] do
										if  p1 > 0 then
											begin
											p1:= p1 - 250 - randint(250);
											if (p1 < 1) then
												p1:= 1;
											msg_print('Your light dims...')
											end
									end;
{ Eat charges }					24:
									if  inven_ctr > 0 then
										begin
										i1:= randint(inven_ctr);
										InvenAbsIdxToTypeIdx(i1, t, i1);
										with newInventory[t, i1] do
											if (tval in [55, 60, 65]) then
												if (p1 > 0) then
													begin
													hp:= hp + 5 * p1;
													p1:= 0;
													msg_print('Energy drains from your pack!');
													end
										end;
{ Vampire Bite -MWK }			25:
									begin
						{ Lose experience }
									msg_print(
										'You feel your life draining away!');
									i1:= damroll(damstr) +
											(py.misc.exp div 100) * mon_drain_life;
									lose_exp(i1);
						{ Poison Strength }
									take_hit(damroll(damstr), ddesc);
									if (py.flags.sustain_str) then
										msg_print(
											'You feel weaker for a moment, then it passes.')
									else if (randint(2) = 1) then
										begin
										msg_print('You feel weaker.');
										py.stat.cstr:=
												de_statp(py.stat.cstr);
										prt_strength
										end;
									prt_chp
									end;
{ Turn to stone -Opusii }		26:
									if (randint(50) > 47) then
										begin
										msg_print(
											'Your joints feel stiff.');
										py.misc.chp:= -1;
										prt_chp;
										msg_print(
											'You have been turned to stone.');
										msg_print('');
										take_hit(0, ddesc)
										end;
{ Mind Flayer }					27: { Need to check for dexterity }
									if (randint(50) > 47) then
										begin
										msg_print(
											'The mind flayer''s tentacles touch you.');
										py.misc.chp:= -1;
										prt_chp;
										msg_print(
											'Your brain has been sucked out.');
										msg_print('');
										take_hit(0, ddesc)
										end;
{ Hell Horse/Nightmare }		28:
									begin
									with py.flags do
										begin
										blind:= blind + randint(5);
										end;
									msg_print('Your eyes sting.');
									take_hit(damroll(damstr), ddesc);
									end;
{ Vargouillian Attack-NBT }		29:
									begin
		  { Lose Max H.P. by 1d6 }
									take_hit(damroll(damstr), ddesc);
									if (py.flags.sustain_str) then

{ I fails save vs poison, nail the muther }

										msg_print('You feel weaker for a moment, then it passes.')
									else if (randint(3) > 1) then
										begin
										msg_print(
											'Your hit points are drained!');
										with py.misc do
											begin
											mhp:= mhp - randint(6);
											if (mhp < chp) then
												chp:= mhp;
											prt_mhp;
											prt_chp;
											end;
										end;
									prt_chp
									end;
								99:
									;
								else
								end
							end
						else
							case adesc of
								1, 2, 3, 6:
									msg_print(cdesc + 'misses you.');
								else
								end
						end
					end
			end;

 { Make the move if possible, five choices }
		function make_move(monptr: integer; mm: mm_type): boolean;
			var
			i1, i2, newy, newx: integer;
			movebits:           unsigned;
			flag, tflag:        boolean;

			begin
			i1       := 1;
			flag     := false;
			make_move:= false;
			movebits := c_list[m_list[monptr].mptr].cmove;
			repeat
{ Get new positon }
				newy:= m_list[monptr].fy;
				newx:= m_list[monptr].fx;
				move(mm[i1], newy, newx);
				with cave[newy, newx] do
					if (fval <> 15) then
						begin
						tflag:= false;
{ Floor is open? }
						if (fopen) then
							tflag:= true
{ Creature moves through walls? }
						else if ((movebits and $40000) <> 0) then
							tflag:= true
{ Creature can open doors? }
						else if (tptr > 0) then
							with t_list[tptr] do
								with m_list[monptr] do
									if ((movebits and $20000) <> 0)
									then
										begin
{ Creature can open doors. }
										case tval of
											105:
												begin
{ Closed doors. } 								if (p1 = 0) then
													begin
													tflag:= true;
													if (fm) then
														if (los(char_row, char_col, newy,newx)) then
															begin
															t_list[tptr]:= door_list[1];
															fopen:= true;
															lite_spot(newy, newx);
															tflag:= false
															end
													end
{ Locked door } 								else if (p1 > 0) then
													begin
													if (randint(100 - level) < 5) then
														p1:= 0
													end
{ Stuck doors } 								else if (p1 < 0) then
													begin
													if (randint(hp) > (10 + abs(p1))) then
														p1:= 0
													end
												end;
{ Secret doors.. }							109:
												begin
												tflag:= true;
												if (fm) then
													if (los(char_row, char_col, newy, newx)) then
														begin
														t_list[tptr]:= door_list[1];
														fopen:= true;
														lite_spot(newy, newx);
														tflag:= false
														end
												end;
											else
											end
										end
									else
										begin { Creature can not open doors, must bash them }
										case tval of
											105:
												begin
												i2:= abs(p1) + 20;
												if (randint(hp) > i2)
												then
													begin
													tflag:= true;
													if (fm) then
														if (los(char_row,char_col, newy,newx)) then
															begin
															t_list[tptr]:= door_list[1];
															t_list[tptr].p1:= randint(2) - 1;
															fopen:= true;
															lite_spot(newy, newx);
															tflag:= false
															end
													end
												end;
											109:
												;
											else
											end
										end;
{ Glyph of warding present?
Scare Monster trap } 	if (tflag) then
							if (tptr > 0) then
								if (t_list[tptr].tval = 102) then
									if (t_list[tptr].subval = 99) then
										tflag:= false;
{ Creature has attempted to move on player? }
						if (tflag) then
							if (cptr = 1) then
								begin
								if (not(m_list[monptr].ml)) then
									update_mon(monptr);
								if (find_flag) then
									begin
									find_flag:= false;
									move_char(5)
									end;
								make_attack(monptr);
{ Player has read a Confuse Monster. Monster gets a saving throw... }
								if (py.flags.confuse_monster) then
									with m_list[monptr] do
										with c_list[mptr] do
											begin
											msg_print(
												'Your hands stop glowing.');
											py.flags.confuse_monster:=
													false;
											if ((randint(max_mons_level) < level)
											or  (($1000 and cdefense) <> 0))
											then
												begin
												msg_print('The ' + name + ' is unaffected.')
												end
											else
												begin
												msg_print('The ' + name + ' appears confused.');
												confused:= true
												end
											end;
								tflag:= false;
								flag := true
								end
{ Creature is attempting to move on other creature? }
							else if ((cptr > 1)
							and ((newy <> m_list[monptr].fy)
							or   (newx <> m_list[monptr].fx))) then
								begin
{ Creature eats other creatures? }
								if ((movebits and $80000) <> 0) then
									delete_monster(cptr)
								else
									tflag:= false
								end;
{ Creature has been allowed move }
						if (tflag) then
							with m_list[monptr] do
								begin
{ Pick up or eat an object }
								if ((movebits and $100000) <> 0) then
									with cave[newy, newx] do
										if (tptr > 0) then
											if (t_list[tptr].tval < 100) then
												delete_object(newy, newx);
{ Move creature record }
								move_rec(fy, fx, newy, newx);
								fy       := newy;
								fx       := newx;
								flag     := true;
								make_move:= true
								end
						end;
				i1:= i1 + 1;
{ Up to 5 attempts at moving, then give up }
				until ((flag) or (i1 > 5));
			end;

{ Creatures can cast spells too.
cast_spell := true if creature changes position
took_turn  := true if creature casts a spell }
		function cast_spell(monptr: integer;
				var took_turn: boolean): boolean;
			var
			i1: unsigned;
			i2, i3, y, X, chance, thrown_spell: integer;
			r1:                   real;
			spell_choice:         array [1 .. 31] of integer;
			cdesc: vtype;
			ddesc: AnsiString;
			outval: vtype;
			flag:                 boolean;

			begin
			with m_list[monptr] do
				with c_list[mptr] do
					begin
					chance:= {int}((spells and $0000000F));
	{ 1 in x chance of casting spell }
					if (randint(chance) <> 1) then
						begin
						cast_spell:= false;
						took_turn := false
						end
	{ Must be within certain range }
					else if (cdis > max_spell_dis) then
						begin
						cast_spell:= false;
						took_turn := false
						end
	{ Must have unobstructed Line-Of-Sight }
					else if (not(los(char_row, char_col, fy, fx))) then
						begin
						cast_spell:= false;
						took_turn := false
						end
					else
	{ Creature is going to cast a spell }
						begin
						took_turn := true;
						cast_spell:= false;
	{ Describe the attack }
						flag:= true;
						if (not(ml)) then
							flag:= false
						else if ((($10000 and c_list[mptr].cmove) <> 0)
						and (not(py.flags.see_inv))) then
							flag:= false
						else if (py.flags.blind > 0) then
							flag:= false;
						if (flag) then
							cdesc:= 'The ' + name + ' '
						else
							cdesc:= 'It ';
	{ For 'DIED_FROM' string }
						if (($80000000 and cmove) <> 0) then
							ddesc:= 'The ' + name
						else
							ddesc:= '& ' + name;
						invenTemp.name:= ddesc;
						invenTemp.number:= 1;
						objdes(ddesc, @invenTemp, true);
	{ End DIED_FROM. Extract all possible spells into spell_choice }
						i1:= (spells and $FFFFFFF0);
						i3:= 0;
						while (i1 <> 0) do
							begin
							i2              := bit_pos(i1);
							i3              := i3 + 1;
							spell_choice[i3]:= i2
							end;
	{ Choose a spell to cast }
						thrown_spell:= spell_choice[randint(i3)];
	{ Cast the spell... }
						case thrown_spell of
	{ Teleport Short }		5:
								teleport_away(monptr, 4);
	{ Teleport Long }		6:
								teleport_away(monptr, max_sight);
	{ Teleport To }			7:
								begin
								cdesc:= cdesc + 'casts a spell.';
								msg_print(cdesc);
								msg_print(' ');
								teleport_to(fy, fx)
								end;
	{ Light Wound }			8:
								begin
								cdesc:= cdesc + 'casts a spell.';
								msg_print(cdesc);
								if (player_saves(wis_adj +
												py.misc.lev)) then
									msg_print(
										'You resist the affects of the spell.')
								else
									take_hit(damroll('2d8'), ddesc)
								end;
	{ Serious Wound }		9:
								begin
								cdesc:= cdesc + 'casts a spell.';
								msg_print(cdesc);
								if (player_saves(wis_adj +
												py.misc.lev)) then
									msg_print(
										'You resist the affects of the spell.')
								else
									take_hit(damroll('8d8'), ddesc)
								end;
	{ Hold Person }			10:
								begin
								cdesc:= cdesc + 'casts a spell.';
								msg_print(cdesc);
								if (py.flags.free_act) then
									msg_print(
										'You are unaffected...')
								else if (player_saves(wis_adj +
												py.misc.lev)) then
									msg_print(
										'You resist the affects of the spell.')
								else if (py.flags.paralysis > 0)
								then
									py.flags.paralysis:=
											py.flags.paralysis + 2
								else
									py.flags.paralysis:=
											randint(5) + 4
								end;
	{ Cause Blindnes }		11:
								begin
								cdesc:= cdesc + 'casts a spell.';
								msg_print(cdesc);
								if (player_saves(wis_adj +
												py.misc.lev)) then
									msg_print(
										'You resist the affects of the spell.')
								else if (py.flags.blind > 0) then
									py.flags.blind:=
											py.flags.blind + 6
								else
									begin
									py.flags.blind:= py.flags.blind
											+ 12 + randint(3);
									msg_print(' ')
									end
								end;
	{ Cause Confuse }		12:
								begin
								cdesc:= cdesc + 'casts a spell.';
								msg_print(cdesc);
								if (player_saves(wis_adj +
												py.misc.lev)) then
									msg_print(
										'You resist the affects of the spell.')
								else if (py.flags.confused > 0) then
									py.flags.confused:=
											py.flags.confused + 2
								else
									py.flags.confused:=
											randint(5) + 3
								end;
	{ Cause Fear }			13:
								begin
								cdesc:= cdesc + 'casts a spell.';
								msg_print(cdesc);
								if (player_saves(wis_adj +
												py.misc.lev)) then
									msg_print(
										'You resist the affects of the spell.')
								else if (py.flags.afraid > 0) then
									py.flags.afraid:=
											py.flags.afraid + 2
								else
									py.flags.afraid:= randint(5) + 3
								end;
	{ Summon Monster }		14:
								begin
								cdesc:= cdesc +
										'magically summons a monster!';
								msg_print(cdesc);
								y:= char_row;
								X:= char_col;
								summon_monster(y, X, false);
								check_mon_lite(y, X)
								end;
	{ Summon Undead }		15:
								begin
								cdesc:= cdesc +
										'magically summons an undead!';
								msg_print(cdesc);
								y:= char_row;
								X:= char_col;
								summon_undead(y, X);
								check_mon_lite(y, X)
								end;
	{ Slow Person }			16:
								begin
								cdesc:= cdesc + 'casts a spell.';
								msg_print(cdesc);
								if (py.flags.free_act) then
									msg_print(
										'You are unaffected...')
								else if (player_saves(wis_adj +
												py.misc.lev)) then
									msg_print(
										'You resist the affects of the spell.')
								else if (py.flags.slow > 0) then
									py.flags.slow:=
											py.flags.slow + 2
								else
									py.flags.slow:= randint(5) + 3
								end;
	{ Drain Mana }			17:
								if (trunc(py.misc.cmana) > 0) then
									begin
									outval:= cdesc +
											'draws psychic energy from you!';
									msg_print(outval);
									outval:= cdesc +
											'appears stronger...';
									msg_print(outval);
									r1:= randint(level);
									if (r1 > py.misc.cmana) then
										r1:= py.misc.cmana;
									py.misc.cmana:=
											py.misc.cmana - r1;
									hp:= hp + 5 * trunc(r1)
									end;
	{ Breath Light }		20:
								begin
								cdesc:= cdesc +
										'breathes lightning.';
								msg_print(cdesc);
								breath(1, char_row, char_col, (hp div 3), ddesc)
								end;
	{ Breath Gas }			21:
								begin
								cdesc:= cdesc + 'breathes gas.';
								msg_print(cdesc);
								breath(2, char_row, char_col, (hp div 3), ddesc)
								end;
	{ Breath Acid }			22:
								begin
								cdesc:= cdesc + 'breathes acid.';
								msg_print(cdesc);
								breath(3, char_row, char_col, (hp div 3), ddesc)
								end;
	{ Breath Frost }		23:
								begin
								cdesc:= cdesc + 'breathes frost.';
								msg_print(cdesc);
								breath(4, char_row, char_col, (hp div 3), ddesc)
								end;
	{ Breath Fire }			24:
								begin
								cdesc:= cdesc + 'breathes fire.';
								msg_print(cdesc);
								breath(5, char_row, char_col, (hp div 3), ddesc)
								end;
							else
								begin
								msg_print(
									'Creature casts an unknown spell.');
								cdesc:= ''
								end
							end
	{ End of spells }
						end
					end
			end;

 { Main procedure for monster movement (MON_MOVE) }
		begin
		mon_move:= false;
		with c_list[m_list[monptr].mptr] do
			begin
{ Does the critter multiply? }
			if ((cmove and $00200000) <> 0) then
				if (max_mon_mult >= mon_tot_mult) then
					if ((py.flags.rest mod mon_mult_adj) = 0) then
						with m_list[monptr] do
							begin
							i3        := 0;
							for i1    := fy - 1 to fy + 1 do
								for i2:= fx - 1 to fx + 1 do
									if (in_bounds(i1, i2)) then
										if (cave[i1, i2].cptr > 1)
										then
											i3:= i3 + 1;
							if (i3 < 4) then
								if (randint(i3 * mon_mult_adj) = 1)
								then
									multiply_monster(fy, fx,
										mptr, false)
							end;
{ Creature is confused?  Chance it becomes un-confused }
			move_test:= false;
			if (m_list[monptr].confused) then
				begin
				mm[1]   := randint(9);
				mm[2]   := randint(9);
				mm[3]   := randint(9);
				mm[4]   := randint(9);
				mm[5]   := randint(9);
				mon_move:= make_move(monptr, mm);
				if (randint(8) = 1) then
					m_list[monptr].confused:= false;
				move_test                  := true
				end
{ Creature may cast a spell }
			else if (spells > 0) then
				mon_move:= cast_spell(monptr, move_test);
			if (not(move_test)) then
				begin
{ 75% random movement }
				if ((randint(100) < 75) and
							((cmove and 00000020) <> 0)) then
					begin
					mm[1]   := randint(9);
					mm[2]   := randint(9);
					mm[3]   := randint(9);
					mm[4]   := randint(9);
					mm[5]   := randint(9);
					mon_move:= make_move(monptr, mm)
					end
{ 40% random movement }
				else if ((randint(100) < 40) and
							((cmove and 00000010) <> 0)) then
					begin
					mm[1]   := randint(9);
					mm[2]   := randint(9);
					mm[3]   := randint(9);
					mm[4]   := randint(9);
					mm[5]   := randint(9);
					mon_move:= make_move(monptr, mm)
					end
{ 20% random movement }
				else if ((randint(100) < 20) and
							((cmove and $00000008) <> 0)) then
					begin
					mm[1]   := randint(9);
					mm[2]   := randint(9);
					mm[3]   := randint(9);
					mm[4]   := randint(9);
					mm[5]   := randint(9);
					mon_move:= make_move(monptr, mm)
					end
{ Normal movement }
				else if ((cmove and $00000002) <> 0) then
					begin
					if (randint(200) = 1) then
						begin
						mm[1]:= randint(9);
						mm[2]:= randint(9);
						mm[3]:= randint(9);
						mm[4]:= randint(9);
						mm[5]:= randint(9)
						end
					else
						get_moves(monptr, mm);
					mon_move:= make_move(monptr, mm);
					end
{ Attack, but don't move }
				else if ((cmove and 00000001) <> 0) then
					if (m_list[monptr].cdis < 2) then
						begin
						get_moves(monptr, mm);
						mon_move:= make_move(monptr, mm)
						end
				end;
			end;
		end;

{ Main procedure for creatures }
	begin
	if (muptr > 0) then
		begin
{ Process the monsters }
		i1:= muptr;
		repeat
			with m_list[i1] do
				begin
				cdis:= distance(char_row, char_col, fy, fx);
{ Attack is argument passed to CREATURE }
				if (attack) then
					begin
					i3:= movement_rate(cspeed);
					if (i3 > 0) then
						for i2:= 1 to movement_rate(cspeed) do
							begin
							if  ((cdis <= c_list[mptr].aaf)
							or   (ml)) then
								begin
								if (csleep and $8000) <> 0 then
									begin
									tmp:= csleep and $7FFF;
									if tmp <= 1 then
										csleep:= 0
									else
										begin
										Dec(tmp);
										csleep:= $8000 + tmp;
										end;
									end
								else if (csleep > 0) then
									begin
									if (py.flags.aggravate) then
										csleep:= 0
									else if (py.flags.rest < 1) then
										if (randint(10) > py.misc.stl) then
											begin
											tmp:= csleep - (75 div cdis);
											if tmp > 0 then
												csleep:= tmp
											else
												csleep:= 0;
											end;
									end;

								if (stuned > 0) then
									stuned:= stuned - 1;

								if  ((csleep <= 0)
								and  (stuned <= 0)) then
									begin
									moldy:= fy;
									moldx:= fx;
									if (mon_move(i1)) then
										if (ml) then
											begin
											ml:= false;
											if (test_light(moldy, moldx)) then
												lite_spot(moldy, moldx)
											else
												unlite_spot(moldy, moldx)
											end
									end
								end;
							update_mon(i1)
							end
					else
						update_mon(i1)
					end
				else
					update_mon(i1)
				end;
			i1:= m_list[i1].nptr;
			until ((i1 = 0) or (moria_flag));
{ End processing monsters }
		end;
	end;


