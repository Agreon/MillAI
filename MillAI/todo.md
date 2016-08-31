#intro
+ intro-screen
#game
+ AI
	+ JUMP DOES not find optimal
		+ gibt manchmal 10000 zurück obwohl er eine mühle hätte
	+ wenn spieler springen darf, kapiert der gegner das nicht


	+ GEGNER nimmt keinen stein, wenn er gewonnen hätte
		+ liegt am "ist stein teil einer mühle"-check, wenn nurnoch 3 über sind
		=> performance
			+ entweder für jeden stein checken ob stoneAmount = 3 wenn teil einer mühle
			oder jedes mal wenn ein stein entfernt werden soll  

	+ MOVING/JUMPING führt zu unerwarteten ergebnissen

+ funktion
	+ spieler-selektion
		+ ui einbauen
	+ Aufgeben
+ sounds
+ computer
	+ thread-tests
+ Design	
	+ Spielfeld
		+ OnHover andere Farben
		+ [schiebe-anim]
		+ sollte nicht rot markiert werden, wenn nicht removed werden kann
		+ bei schieben sollte ein durchscihtig-grüner gezeigt werden

	+ OnClick ins nix, aktuelle selection aufheben? 
+ extra
	+ player-wechsel funktioniert noch nicht ganz
		+ wenn gegner dran ist, wird nicht beachtet, das spieler noch einen stein legen kann
		=> bei legphase gewonnen
		=> Fixed solange immer player1 beginnt
		+ removeStone
			=> übrige steine noch eins zu hoc#



+ board
	+ fields to 1d-array