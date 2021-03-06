type
	ByteInt         = Byte;
	BytlInt         = ShortInt;
	WordInt         = Word;
	WorlInt         = SmallInt;

//dengland Was defined as LongWord but that would be a problem in 64bit land.
	Unsigned        = Cardinal;

//dengland  In BOSS, this is SmallInt but that doesn't work for the experience
//		table in MORIA (needs a 32 bit number).  Was defined as LongInt but that
//		would be a problem in 64bit land.  Its simply unnecessary.
//	Integer         = LongInt;

	Real            = Single;

//dengland This isn't required anymore.
//	Quad_Type       = record
//		l0: Unsigned;
//		l1: Unsigned;
//		end;

	AType           = string[16];
	BType           = string[14];
	CType           = string[26];
	DType           = string[5];
	EType           = string[60];
	MType           = string[190];
	NType           = AnsiString;
	TType           = string[68];
	VType           = string[80];
	Stat_Type       = packed array [1..6] of AnsiChar;
	Obj_Set         = set of 0..255;

//dengland The Delphi XE2 compiler doesn't like this and its only used somewhere
//		where this makes no difference.  This is probably more valid, anyhow.
//	Char_Set        = set of 'A'..'z';
	Char_Set		= set of AnsiChar;

	Key_Type        = record                { For char saver        }
		file_id : {[key(0)]} packed array [1..70] of AnsiChar;
		seed    : Integer;
		end;

	Creature_Type = record
		name    : ctype;        { Descrip of creature   }
		cmove   : Unsigned;     { Bit field             }
		spells  : Unsigned;     { Creature spells       }
		cdefense: Unsigned;     { Bit field             }
//									$00000001	- Is dragon
//									$00000002	- Is monster
//									$00000004	- Is evil
//									$00000008	- Is undead
//									$00000010	- Harmed by cold
//									$00000020	- Harmed by fire
//									$00000040	- Harmed by poison
//									$00000080	- Harmed by acid
//									$00000100	- Harmed by blue light
//									$00000200	- Harmed by Stone To Mud
//									$00000400	- Player loses experience if harms
//									$00000800	-
//									$00001000	- Immune to confusion (charm/sleep)
//									$00002000	- Can be seen with infravision
//									$00004000	- Has maximum hit points
//									$00008000
//									$00010000
//									$00020000
//									$00040000
//									$00080000
//									$00100000
//									$00200000
//									$00400000
//									$00800000
//									$01000000
//									$02000000
//									$04000000
//									$08000000
//									$10000000
//									$20000000
//									$40000000
//									$80000000

		sleep   : WorlInt;      { Inactive counter      }
		mexp    : WorlInt;      { Exp value for kill    }
		aaf     : ByteInt;      { Area affect radius    }
		ac      : ByteInt;      { AC                    }
		speed   : BytlInt;      { Movement speed        }
		cchar   : AnsiChar;     { Character rep.        }
		hd      : DType;        { Creatures hit die     }
		damage  : EType;        { Type attack and damage}
		level   : ByteInt;      { Level of creature     }
//		csubcls : ByteInt;		{ Sub class of creature }
		end;

	Monster_Type = record
		hp      : WorlInt;      { Hit points            }
//dengland Changed to unsigned for magically asleep bit ($8000)
		csleep  : WordInt;      { Inactive counter      }
		cdis    : WorlInt;      { Cur dis from player   }
		mptr    : WordInt;      { Pointer into creature }
		nptr    : WordInt;      { Pointer to next block }
		cspeed  : BytlInt;      { Movement speed        }

		{ Note: FY and FX constrain dungeon size to 255 }
		fy      : ByteInt;      { Y Pointer into map    }
		fx      : ByteInt;      { X Pointer into map    }

//		[bit(6),pos(104)]
		stuned  : -32..31; { Rounds stunned}
//		[bit(1),pos(110)]
		ml      : Boolean; { On if shown   }
//		[bit(1),pos(111)]
		confused: Boolean; { On if confused}
//DENGLAND
		elite: Boolean; 	{True if elite}
		stoleau: Unsigned;  {Amount of stolen gold}
		end;

	PTreasure_Type = ^Treasure_Type;
	Treasure_Type = record
		name    : TType;        { Object name           }
		tval    : ByteInt;      { Catagory number       }
		tchar   : AnsiChar;     { Character representation}
		flags   : Unsigned;     { Special flags         }
								{ $00000001 - +strength 		}
								{ $00000002 - +dexterity		}
								{ $00000004 - +constitution		}
								{ $00000008 - +intelligence		}
								{ $00000010 - +wisdom	 		}
								{ $00000020 - +charisma 		}
								{ $00000020 - +charisma 		}
								{ $00000040 - +search chance&freq}
								{ $00000080 - +slow digestion 	}
								{ $00000100 - +stealth	 		}
								{ $00000200 - aggravating 		}
								{ $00000400 - teleporting 		}
								{ $00000800 - +regeneration		}
								{ $00001000 - +speed		 	}
								{ $00002000 - dragon slaying	}
								{ $00004000 - monster slaying	}
								{ $00008000 - evil slaying		}
								{ $00010000 - undead slaying	}
								{ $00020000 - +frost dmg		}
								{ $00040000 - +fire dmg			}
								{ $00080000 - +fire resist		}
								{ $00100000 - +acid resist		}
								{ $00200000 - +cold resist		}
								{ $00400000 - sustain all stats }
								{ $00800000 - free acting		}
								{ $01000000 - see invisible		}
								{ $02000000 - +light resist		}
								{ $04000000 - feather fall		}
								{ $08000000 - blinding	 		}
								{ $10000000 - feared			}
								{ $20000000 - +to digging		}
								{ $40000000 - grant infravision }
								{ $80000000 - cursed			}
		p1      : Integer;      { Misc. use variable    }
		cost    : Integer;      { Cost of item          }
		subval  : Integer;      { Sub-catagory number   }
		weight  : WordInt;      { Weight                }
		number  : WordInt;      { Number of items       }
		tohit   : WorlInt;      { Pluses to hit         }
		todam   : WorlInt;      { Pluses to damage      }
		ac      : WorlInt;      { Normal AC             }
		toac    : WorlInt;      { Pluses to AC          }
		damage  : DType;        { Damage when hits      }
		level   : BytlInt;      { Level item found      }
		end;

	Player_Type = record
		misc : record
			name    : vtype;        { Name of character }
			race    : vtype;        { Race of character }
			sex     : vtype;        { Sex of character }
			title   : vtype;        { Character's title }
			tclass  : vtype;        { Character's class }
			max_exp : Integer;      { Max experience}
			exp     : Integer;      { Cur experienc }
			au      : Integer;      { Gold }
			age     : WordInt;      { Characters age}
			ht      : WordInt;      { Height }
			wt      : WordInt;      { Weight }
			lev     : WordInt;      { Level }
			max_lev : WordInt;      { Max level explored}
			srh     : WorlInt;      { Chance in search}
			fos     : WorlInt;      { Frenq of search}
			bth     : WorlInt;      { Base to hit }
			bthb    : WorlInt;      { BTH with bows }
			mana    : WorlInt;      { Mana points }
			mhp     : WorlInt;      { Max hit pts }
			ptohit  : WorlInt;      { Pluses to hit }
			ptodam  : WorlInt;      { Pluses to dam }
			pac     : WorlInt;      { Total AC }
			ptoac   : WorlInt;      { Magical AC }
			dis_th  : WorlInt;      { Display +ToHit}
			dis_td  : WorlInt;      { Display +ToDam}
			dis_ac  : WorlInt;      { Display +ToAC }
			dis_tac : WorlInt;      { Display +ToTAC}
			disarm  : WorlInt;      { % to Disarm }
			save    : WorlInt;      { Saving throw }
			sc      : WorlInt;      { Social Class }
			pclass  : ByteInt;      { # of class }
			prace   : ByteInt;      { # of race }
			hitdie  : ByteInt;      { Char hit die }
			stl     : BytlInt;      { Stealth factor}
			expfact : Real;         { Experience factor}
			cmana   : Real;         { Cur mana pts }
			chp     : Real;         { Cur hit pts }
			history : array [1..5] of VType;{ History record}
			end;
		stat : record
			str     : ByteInt;      { Max strength        }
			cstr    : ByteInt;      { Current strength    }
			astr:     ByteInt;      { Strength adjustment }
			dex     : ByteInt;      { Max dexterity       }
			cdex    : ByteInt;      { Current dexterity   }
			adex:     ByteInt;
			con     : ByteInt;      { Max constitution    }
			ccon    : ByteInt;      { Current constitution}
			acon:     ByteInt;
			int     : ByteInt;      { Max intelligence    }
			cint    : ByteInt;      { Current intelligence}
			aint:     ByteInt;
			wis     : ByteInt;      { Max wisdom          }
			cwis    : ByteInt;      { Current wisdom      }
			awis:     ByteInt;
			chr     : ByteInt;      { Max charisma        }
			cchr    : ByteInt;      { Current charisma    }
			achr:     ByteInt;
//dengland
			avail:    Unsigned;		{ Available points    }
			end;
		flags : record
			status          : Unsigned;     { Status of player    }
											{ $00000001 = Hungry  }
											{ $00000002 = Weak    }
											{ $00000004 = Blind   }
											{ $00000008 = Confused}
											{ $00000010 = Afraid  }
											{ $00000020 = Poisoned}
											{ $00000040 = Fast    }
											{ $00000080 = Slow    }
											{ $00000100 = Searching}
											{ $00000200 = Resting }
											{ $00000400 =         }
											{ $00000800 =         }
											{ $00001000 = Invunerable }
											{ $00002000 = Hero    }
											{ $00004000 = Superhero }
											{ $00008000 = Blessed }
											{ $00010000 = Detect invisible }
											{ $00020000 = Timed infra-vision }
//dengland
											{ $01000000 = Softcore }
											{ $02000000 = Easy }
											{ $04000000 = Cheating }
											{ $08000000 = Cheater }

			rest            : Integer;      { Rest counter        }
			blind           : Integer;      { Blindness counter   }
			paralysis       : Integer;      { Paralysis counter   }
			confused        : Integer;      { Confusion counter   }
			food            : Integer;      { Food counter        }
			food_digested   : Integer;      { Food per round      }
			protection      : Integer;      { Protection fr. evil }
			speed           : Integer;      { Cur speed adjust    }
			fast            : Integer;      { Temp speed change   }
			slow            : Integer;      { Temp speed change   }
			afraid          : Integer;      { Fear                }
			poisoned        : Integer;      { Poisoned            }
			image           : Integer;      { Halucinate          }
			protevil        : Integer;      { Protect VS evil     }
			invuln          : Integer;      { Increases AC        }
			hero            : Integer;      { Heroism             }
			shero           : Integer;      { Super Heroism       }
			blessed         : Integer;      { Blessed             }
			resist_heat     : Integer;      { Timed heat resist   }
			resist_cold     : Integer;      { Timed cold resist   }
			detect_inv      : Integer;      { Timed see invisible }
			word_recall     : Integer;      { Timed teleport level}
			see_infra       : Integer;      { See warm creatures  }
			tim_infra       : Integer;      { Timed infra vision  }
			see_inv         : Boolean;      { Can see invisible   }
			teleport        : Boolean;      { Random teleportation}
			free_act        : Boolean;      { Never paralyzed     }
			slow_digest     : Boolean;      { Lower food needs    }
			aggravate       : Boolean;      { Agravate monsters   }
			fire_resist     : Boolean;      { Resistance to fire  }
			cold_resist     : Boolean;      { Resistance to cold  }
			acid_resist     : Boolean;      { Resistance to acid  }
			regenerate      : Boolean;      { Regenerate hit pts  }
			lght_resist     : Boolean;      { Resistance to light }
			ffall           : Boolean;      { No damage falling   }
			sustain_str     : Boolean;      { Keep strength       }
			sustain_int     : Boolean;      { Keep intelligence   }
			sustain_wis     : Boolean;      { Keep wisdom         }
			sustain_con     : Boolean;      { Keep constitution   }
			sustain_dex     : Boolean;      { Keep dexterity      }
			sustain_chr     : Boolean;      { Keep charisma       }
			confuse_monster : Boolean;      { Glowing hands...    }
			end;
		end;

	Spell_Type = record
		sname   : CType;
		slevel  : ByteInt;
		smana   : ByteInt;
		sexp    : WordInt;
		sfail   : ByteInt;
		learned : Boolean;
		end;

	Spl_Rec = record
		splnum  : Integer;
		splchn  : Integer;
		end;

	Spl_Type = array [1..22] of Spl_Rec;

	Race_Type = packed record
		trace   : vtype;        { Type of race }
		str_adj : BytlInt;      { adjustments }
		int_adj : BytlInt;
		wis_adj : BytlInt;
		dex_adj : BytlInt;
		con_adj : BytlInt;
		chr_adj : BytlInt;
		b_age   : WordInt; { Base age of character }
		m_age   : WordInt; { Maximum age of character }
		m_b_ht  : WordInt; { base height for males }
		m_m_ht  : WordInt; { mod height for males       }
		m_b_wt  : WordInt; { base weight for males      }
		m_m_wt  : WordInt; { mod weight for males       }
		f_b_ht  : WordInt; { base height females        }
		f_m_ht  : WordInt; { mod height for females     }
		f_b_wt  : WordInt; { base weight for female     }
		f_m_wt  : WordInt; { mod weight for females     }
		b_exp   : Real;    { Base experience factor     }
		b_dis   : BytlInt; { base chance to disarm      }
		srh     : BytlInt; { base chance for search     }
		stl     : BytlInt; { Stealth of character       }
		fos     : BytlInt; { frequency of auto search   }
		bth     : BytlInt; { adj base chance to hit     }
		bthb    : BytlInt; { adj base to hit with bows  }
		bsav    : BytlInt; { Race base for saving throw }
		bhitdie : BytlInt; { Base hit points for race   }
		infra   : BytlInt; { See infra-red              }
		tclass  : Unsigned; { Bit field for class types }
		end;

	Class_Type = record
		title   : VType;  { type of class }
		m_exp   : Real;   { Class experience factor }
		adj_hd  : BytlInt;{ Adjust hit points }
		mdis    : BytlInt;{ mod disarming traps }
		msrh    : BytlInt;{ modifier to searching }
		mstl    : BytlInt;{ modifier to stealth }
		mfos    : BytlInt;{ modifier to freq-of-search  }
		mbth    : BytlInt;{ modifier to base to hit     }
		mbthb   : BytlInt;{ modifier to base to hit - bows}
		msav    : BytlInt;{ Class modifier to save      }
		madj_str: BytlInt;{ Class modifier for strength}
		madj_int: BytlInt;{ Class modifier for intelligence}
		madj_wis: BytlInt;{ Class modifier for wisdom   }
		madj_dex: BytlInt;{ Class modifier for dexterity}
		madj_con: BytlInt;{ Class modifier for constitution}
		madj_chr: BytlInt;{ Class modifier for charisma}
		pspell  : Boolean;{ class use priest spells    }
		mspell  : Boolean;{ class use mage spells      }
		espell  : Boolean;{ class use extra spells}
		espell_type :Integer;
		espell_book :Integer;
		end;

	Background_Type = record
		info    : vtype;  { History information        }
		roll    : ByteInt;{ Die roll needed for history}
		chart   : ByteInt;{ Table number             }
		next    : BytlInt;{ Pointer to next table       }
		bonus   : BytlInt;{ Bonus to the Social Class   }
		end;

	Floor_Type = record
//		[bit(7),pos(0)]
		ftval   : 0..15;
//		[bit(1),pos(7)]
		ftopen  : Boolean;
		end;

	Cave_Type = record
		cptr    : ByteInt;		//Creature index
		tptr    : ByteInt;		//Treasure index
//		[bit(4),pos(16)]
		fval    : 0..15;		//Floor/wall type
//		[bit(1),pos(20)]
		fopen   : Boolean;		//Can/can't pass through
//		[bit(1),pos(21)]
		fm      : Boolean;		//"Field marker" (runes, rubble, traps, chests??)
//		[bit(1),pos(22)]
		pl      : Boolean;		//Permanent light
//		[bit(1),pos(23)]
		tl      : Boolean;		//Torch light?  (Magically detected objects/treasures
								//and lamp/torch light)
		end;

	Row_Floor  = array [1..MAX_WIDTH] of Cave_Type;

	Owner_Type = record
		owner_name      : vtype;
		max_cost        : Integer;
		max_inflate     : Real;
		min_inflate     : Real;
		haggle_per      : Real;
		owner_race      : ByteInt;
		insult_max      : ByteInt;
		end;

	Inven_Record = record
		scost           : Integer;
		sitem           : Treasure_Type;
		end;

	Store_Type = record
		store_open      : WorlInt;
		owner           : ByteInt;
		insult_cur      : BytlInt;
		store_ctr       : ByteInt;
		store_inven     : array [1..STORE_INVEN_MAX] of Inven_Record;
		end;

	TMoriaCreateStep = (mcsCancel, mcsChooseRace, mcsChooseSex, mcsChooseClass,
			mcsRollStats, mcsGetName, mcsComplete);

	TMoriaStatGrade = (msgLessThan, msgEqualTo, msgGreaterThan);

	TMoriaInvenType = (mitWeapons, mitArmour, mitAccessories, mitAmmunition,
			mitDevices, mitConsumables, mitGeneral, mitBooks, mitWornWielding);
	TMoriaInvenTypes = set of TMoriaInvenType;

	TMoriaInvenWear = (miwMainWeapon, miwHelmet, miwAmulet, miwArmour,
			miwBracers, miwGloves, miwRightRing, miwLeftRing, miwBoots,
			miwCloak, miwLight, miwSecondaryWeapon);

	TMoriaInventory = array[TMoriaInvenType] of array of Treasure_Type;

	TMoriaInvenFunc = (mifDispFull, mifDispEquipped, mifTakeOff, mifWear,
			mifSwitch, mifDispPartial);

	TMoriaLogReason = (mlrInform, mlrWarn, mlrError);

	EMoriaTerminate = EAbort;

