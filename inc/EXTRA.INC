{ Throw a magic spell }
//[psect(misc2$code)]
procedure extra_cast;
	var
	i1,
//	i2,
	item_val,
	dir: integer;
	choice, chance:        integer;
	dumy, y_dumy, x_dumy:  integer;
	redraw:                boolean;
	t: TMoriaInvenType;

	begin
	reset_flag:= true;
	if (py.flags.blind > 0) then
		msg_print('you can''t see to read your spell book!')
	else if (no_light) then
		msg_print('You have no light to read by.')
	else if (py.flags.confused > 0) then
		msg_print('You are too confused...')
	else if (class_[py.misc.pclass].espell) then
//		if (inven_ctr > 0) then
			begin
//			if (find_range([92], i1, i2)) then
//				begin
//				redraw:= false;
//				if (get_item(item_val, 'Use Which Spell Book?', redraw, i1, i2)) then
				if  GetItemFromInv(t, item_val, 'Use which spell book?',
						'But you are not carrying any spell books!', redraw, [92]) then
					begin
					if (cast_spell('Cast which spell?', item_val, choice,
							chance, redraw)) then
						with extra_spell[py.misc.pclass, choice] do
							begin
							reset_flag:= false;
							if (randint(100) < chance) then
								msg_print('You failed to get the spell off!')
							else
								begin
								y_dumy:= char_row;
								x_dumy:= char_col;
{ Spells. }
								case choice of
									1:
										if (get_dir('Which direction?', dir,
												dumy, y_dumy, x_dumy)) then
											fire_bolt(0, dir, char_row,
												char_col, damroll('2d6') + 1,
												'Magic missile');
									2:
										detect_monsters;
									3:
										teleport(10);
									4:
										light_area(char_row, char_col);
									5:
										hp_player(damroll('4d4'),
											'a magic spell.');
									6:
										begin
										detect_sdoor;
										detect_trap;
										end;
									7:
										if (get_dir('Which direction?', dir,
												dumy, y_dumy, x_dumy)) then
											fire_ball(2, dir, char_row,
												char_col, 9, 'Stinking cloud');
									8:
										if (get_dir('Which direction?', dir,
												dumy, y_dumy, x_dumy)) then
											confuse_monster(dir, char_row,
												char_col);
									9:
										if (get_dir('Which direction?', dir,
												dumy, y_dumy, x_dumy)) then
											fire_bolt(1, dir, char_row,
												char_col, damroll('3d8') + 1,
												'Lightning bolt');
									10:
										td_destroy;
									11:
										if (get_dir('Which direction?', dir,
												dumy, y_dumy, x_dumy)) then
											sleep_monster(dir, char_row,
												char_col);
									12:
										cure_poison;
									13:
										teleport(py.misc.lev * 5);
									14:
										for i1:= Ord(Low(TMoriaInvenWear)) to Ord(High(TMoriaInvenWear)) do
											with newInventory[mitWornWielding, i1] do
												flags:= (flags and $7FFFFFFF);
									15:
										if (get_dir('Which direction?', dir,
												dumy, y_dumy, x_dumy)) then
											fire_bolt(4, dir, char_row,
												char_col, damroll('4d8') + 1,
												'Frost bolt');
									16:
										if (get_dir('Which direction?', dir,
												dumy, y_dumy, x_dumy)) then
											wall_to_mud(dir, char_row,
												char_col);
									17:
										ident_spell;
									18:
										recharge(20);
									19:
										sleep_monsters2_3(char_row,
											char_col, 1);
									20:
										if (get_dir('Which direction?', dir,
												dumy, y_dumy, x_dumy)) then
											poly_monster(dir, char_row,
												char_col);
									21:
										create_food;
									22:
										sleep_monsters2_3(char_row,
											char_col, 20);
									23:
										if (get_dir('Which direction?', dir,
												dumy, y_dumy, x_dumy)) then
											fire_bolt(5, dir, char_row,
												char_col, damroll('6d8') + 1,
												'Fire bolt');
									24:
										if (get_dir('Which direction?', dir,
												dumy, y_dumy, x_dumy)) then
											speed_monster(dir, char_row,
												char_col, -1);
									25:
										if (get_dir('Which direction?', dir,
												dumy, y_dumy, x_dumy)) then
											fire_ball(4, dir, char_row,
												char_col, 33, 'Frost ball');
									26:
										recharge(50);
									27:
										if (get_dir('Which direction?', dir,
												dumy, y_dumy, x_dumy)) then
											teleport_monster(dir, char_row,
												char_col);
									28:
										with py.flags do
											fast:= fast + randint(20) +
													py.misc.lev;
									29:
										if (get_dir('Which direction?', dir,
												dumy, y_dumy, x_dumy)) then
											fire_ball(5, dir, char_row,
												char_col, 49, 'Fire ball');
									30:
										destroy_area(char_row, char_col);
									31:
										genocide;
									else
										;
									end;
 { END of spells. }
								if (not(reset_flag)) then
									with py.misc do
										begin
										exp:= exp + sexp;
										prt_experience;
										sexp:= 0;
										end
								end;
							with py.misc do
								if (not(reset_flag)) then
									begin
									if (smana > cmana) then
										begin
										msg_print('You faint from the effort!');
										py.flags.paralysis:=
												randint(5 *
													trunc(smana - cmana));
										cmana:= 0;
										if (randint(3) = 1) then
											begin
											msg_print(
												'You have damaged your health!');
											py.stat.ccon:=
													de_statp(py.stat.ccon);
											prt_constitution;
											end;
										end
									else
										cmana:= cmana - smana;
									prt_mana;
									end
							end
					end
				else if (redraw) then
					draw_cave;
//				end
//			else
//				msg_print('But you are not carrying any spell books!');
			end
//		else
//			msg_print('But you are not carrying any spell books!')
	else
		msg_print('You can''t cast spells!');
	end;
