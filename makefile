Spell_Checker: main.cpp TimeInterval.h
	g++ main.cpp -o Spell_Checker

clean:
	rm Spell_Checker

run: Spell_Checker
	./Spell_Checker
