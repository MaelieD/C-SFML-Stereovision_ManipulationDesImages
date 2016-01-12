# To build the executable (name : monProg) we need the object files (we list them all)
sfml_final : sfml_final.o libImageManip.o libStereo.o
	gcc -Wall -Wextra sfml_final.o libImageManip.o libStereo.o -lm -lcsfml-system -lcsfml-graphics -lcsfml-window -o sfml_final.exe

# To create sfml_fusion.o, we need the source file sfml_fusion.c
sfml_final.o : sfml_final.c
	gcc -Wall -c sfml_final.c

# To create libImageManip.o, we need the source file libImageManip.c
libImageManip.o : libImageManip.c
	gcc -Wall -c libImageManip.c

# To create libStereo.o, we need the source file libStereo.c
libStereo.o : libStereo.c
	gcc -Wall -c libStereo.c
