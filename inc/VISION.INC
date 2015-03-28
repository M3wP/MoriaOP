{
 Following code is taken from early version of my other unfinished
 roguelike game. It was written when I was still learning to program
 in in Free Pascal. The code piece rivals BOSS in ugliness.  -MB

 TODO: Export octant differences into a neat table.
 TODO: Clean up ShadowCast subroutine.
}

function fixy(y: Integer): Integer; inline;
	begin
	if y < panel_row_min then
		fixy:= panel_row_min
	else if y > panel_row_max then
		fixy:= panel_row_max
	else
		fixy:= y
	end;

function fixx(x: Integer): Integer; inline;
	begin
	if x < panel_col_min then
		fixx:= panel_col_min
	else if x > panel_col_max then
		fixx:= panel_col_max
	else
		fixx:= x
	end;

procedure fov(y, x, r, r_limited: Byte);

   { Return true for tile obscuring sight. }
		function blocks(x, y, r: Byte): Boolean;
			const
			door_set = [103, 105, 109]; { Rubble, closed and secret doors. }

			begin
			if (r > r_limited) and not(cave[y, x].fval = 2) then
				exit(true);
			if (cave[y, x].fval = 2) then
				cave[y, x].pl:= true;
			{ Permanently remember seen lit room squares. }
			if (cave[y, x].fval = 5) then { An empty doorway.  Light it only if it
		is part of lit room.  It is safe to check all neighbors because
		junction points are not generated on map borders. }
				if (cave[y - 1, x].fval = 2)
				or (cave[y, x - 1].fval = 2)
				or (cave[y + 1, x].fval = 2)
				or (cave[y, x + 1].fval = 2) then
					cave[y, x].pl:= true;
			Result:= cave[y, x].fval in pwall_set;
			if not Result and (cave[y, x].tptr > 0) then
				Result:= t_list[cave[y, x].tptr].tval in door_set;
			end;

		procedure ShadowCast(Oct, StDep: Byte; StSlop, EdSlop: Single);
 {
  Implements shadow casting for each of eight octants:

  \111|222/
  8\11|22/3
  88\1|2/33
  888\|/333
  ----@----   Lines marked by "|", "-", "/" and "\" are shared edges.
  777/|\444
  77/6|5\44
  7/66|55\4
  /666|555\
 }
			var
			A, B:   SmallInt;
			Shot:   Boolean;
			Fr, Lt: SmallInt;
			dist:   SmallInt;

			const
			Dst = 1 / 65;
			No  = 32;
			Vr1 = No * Dst;
			Vr2 = (No + 1) * Dst;

			begin
			case Oct of
				1:
					begin
					for A:= y - StDep downto fixy(y - r) do
						begin
						Shot:= false;
						Fr  := fixx(x + Round((A - y) * StSlop));
						Lt  := fixx(x + Round((A - y) * EdSlop));
						if Lt < Fr then
							Lt:= Fr;
						for B := Fr to Lt do
							begin
							if B - x > y - A then
								dist:= B - x
							else
								dist     := y - A;
							cave[A, B].tl:= true;
							if blocks(B, A, dist) and (B = Fr) then
								Shot:= true;
							if blocks(B, A, dist) and (Shot = false) then
								begin
								Shot:= true;
								ShadowCast(Oct, y - A + 1, StSlop,
									(x - B + Vr2) / (y - A - Vr1))
								end;
							if blocks(B, A, dist) and (B = Lt) then
								exit;
							if (not blocks(B, A, dist)) and (Shot = true) then
								begin
								Shot  := false;
								StSlop:= (x - B + Vr1) / (y - A + Vr1);
								end
							end
						end
					end;
				2:
					begin
					for A:= y - StDep downto fixy(y - r) do
						begin
						Shot:= false;
						Fr  := fixx(x - Round((A - y) * StSlop));
						Lt  := fixx(x - Round((A - y) * EdSlop));
						if Fr < Lt then
							Fr:= Lt;
						for B := Fr downto Lt do
							begin
							if x - B > y - A then
								dist:= x - B
							else
								dist     := y - A;
							cave[A, B].tl:= true;
							if blocks(B, A, dist) and (B = Fr) then
								Shot:= true;
							if blocks(B, A, dist) and (Shot = false) then
								begin
								Shot:= true;
								ShadowCast(Oct, y - A + 1, StSlop,
									(B - x + Vr2) / (y - A - Vr1))
								end;
							if blocks(B, A, dist) and (B = Lt) then
								exit;
							if (not blocks(B, A, dist)) and (Shot = true) then
								begin
								Shot  := false;
								StSlop:= (B - x + Vr1) / (y - A + Vr1)
								end
							end
						end
					end;
				3:
					begin
					for A:= x + StDep to fixx(x + r) do
						begin
						Shot:= false;
						Fr  := fixy(y - Round((A - x) * StSlop));
						Lt  := fixy(y - Round((A - x) * EdSlop));
						if Lt < Fr then
							Lt:= Fr;
						for B := Fr to Lt do
							begin
							if y - B > A - x then
								dist:= y - B
							else
								dist     := A - x;
							cave[B, A].tl:= true;
							if blocks(A, B, dist) and (B = Fr) then
								Shot:= true;
							if blocks(A, B, dist) and (Shot = false) then
								begin
								Shot:= true;
								ShadowCast(Oct, A - x + 1, StSlop,
									(y - B + Vr2) / (A - x - Vr1))
								end;
							if blocks(A, B, dist) and (B = Lt) then
								exit;
							if (not blocks(A, B, dist)) and (Shot = true) then
								begin
								Shot  := false;
								StSlop:= (y - B + Vr1) / (A - x + Vr1)
								end
							end
						end
					end;
				4:
					begin
					for A:= x + StDep to fixx(x + r) do
						begin
						Shot:= false;
						Fr  := fixy(y + Round((A - x) * StSlop));
						Lt  := fixy(y + Round((A - x) * EdSlop));
						if Fr < Lt then
							Fr:= Lt;
						for B := Fr downto Lt do
							begin
							if B - y > A - x then
								dist:= B - y
							else
								dist     := A - x;
							cave[B, A].tl:= true;
							if blocks(A, B, dist) and (B = Fr) then
								Shot:= true;
							if blocks(A, B, dist) and (Shot = false) then
								begin
								Shot:= true;
								ShadowCast(Oct, A - x + 1, StSlop,
									(B - y + Vr2) / (A - x - Vr1))
								end;
							if blocks(A, B, dist) and (B = Lt) then
								exit;
							if (not blocks(A, B, dist)) and (Shot = true) then
								begin
								Shot  := false;
								StSlop:= (B - y + Vr1) / (A - x + Vr1)
								end
							end
						end
					end;
				5:
					begin
					for A:= y + StDep to fixy(y + r) do
						begin
						Shot:= false;
						Fr  := fixx(x + Round((A - y) * StSlop));
						Lt  := fixx(x + Round((A - y) * EdSlop));
						if Fr < Lt then
							Fr:= Lt;
						for B := Fr downto Lt do
							begin
							if B - x > A - y then
								dist:= B - x
							else
								dist     := A - y;
							cave[A, B].tl:= true;
							if blocks(B, A, dist) and (B = Fr) then
								Shot:= true;
							if blocks(B, A, dist) and (Shot = false) then
								begin
								Shot:= true;
								ShadowCast(Oct, A - y + 1, StSlop,
									(B - x + Vr2) / (A - y - Vr1))
								end;
							if blocks(B, A, dist) and (B = Lt) then
								exit;
							if (not blocks(B, A, dist)) and (Shot = true) then
								begin
								Shot  := false;
								StSlop:= (B - x + Vr1) / (A - y + Vr1)
								end
							end
						end
					end;
				6:
					begin
					for A:= y + StDep to fixy(y + r) do
						begin
						Shot:= false;
						Fr  := fixx(x - Round((A - y) * StSlop));
						Lt  := fixx(x - Round((A - y) * EdSlop));
						if Lt < Fr then
							Lt:= Fr;
						for B := Fr to Lt do
							begin
							if x - B > A - y then
								dist:= x - B
							else
								dist     := A - y;
							cave[A, B].tl:= true;
							if blocks(B, A, dist) and (B = Fr) then
								Shot:= true;
							if blocks(B, A, dist) and (Shot = false) then
								begin
								Shot:= true;
								ShadowCast(Oct, A - y + 1, StSlop,
									(x - B + Vr2) / (A - y - Vr1))
								end;
							if blocks(B, A, dist) and (B = Lt) then
								exit;
							if (not blocks(B, A, dist)) and (Shot = true) then
								begin
								Shot  := false;
								StSlop:= (x - B + Vr1) / (A - y + Vr1)
								end
							end
						end
					end;
				7:
					begin
					for A:= x - StDep downto fixx(x - r) do
						begin
						Shot:= false;
						Fr  := fixy(y - Round((A - x) * StSlop));
						Lt  := fixy(y - Round((A - x) * EdSlop));
						if Fr < Lt then
							Fr:= Lt;
						for B := Fr downto Lt do
							begin
							if B - y > x - A then
								dist:= B - y
							else
								dist     := x - A;
							cave[B, A].tl:= true;
							if blocks(A, B, dist) and (B = Fr) then
								Shot:= true;
							if blocks(A, B, dist) and (Shot = false) then
								begin
								Shot:= true;
								ShadowCast(Oct, x - A + 1, StSlop,
									(B - y + Vr2) / (x - A - Vr1))
								end;
							if blocks(A, B, dist) and (B = Lt) then
								exit;
							if (not blocks(A, B, dist)) and (Shot = true) then
								begin
								Shot  := false;
								StSlop:= (B - y + Vr1) / (x - A + Vr1)
								end
							end
						end
					end;
				8:
					begin
					for A:= x - StDep downto fixx(x - r) do
						begin
						Shot:= false;
						Fr  := fixy(y + Round((A - x) * StSlop));
						Lt  := fixy(y + Round((A - x) * EdSlop));
						if Lt < Fr then
							Lt:= Fr;
						for B := Fr to Lt do
							begin
							if B - y > x - A then
								dist:= B - y
							else
								dist     := x - A;
							cave[B, A].tl:= true;
							if blocks(A, B, dist) and (B = Fr) then
								Shot:= true;
							if blocks(A, B, dist) and (Shot = false) then
								begin
								Shot:= true;
								ShadowCast(Oct, x - A + 1, StSlop,
									(y - B + Vr2) / (x - A - Vr1))
								end;
							if blocks(A, B, dist) and (B = Lt) then
								exit;
							if (not blocks(A, B, dist)) and (Shot = true) then
								begin
								Shot  := false;
								StSlop:= (y - B + Vr1) / (x - A + Vr1)
								end
							end
						end
					end
				end
			end;

	var
	n: Byte;

	begin
	for n:= 1 to 8 do
		ShadowCast(n, 1, 1, 0);
	cave[y, x].tl:= true
	end;

procedure clear_fov(y, x, r: Byte);
	var
	A, B: Byte;

	begin
	for A:= fixy(y - r) to fixy(y + r) do
		for B:= fixx(x - r) to fixx(x + r) do
			cave[A, B].tl:= false
	end;
