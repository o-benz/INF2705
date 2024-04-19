zip -r INF2705_remise_tp1.zip *.cpp *.h *.glsl makefile *.txt shaders  && echo "remettre fichier INF2705_remise_tp1.zip fait avec zip"
IF %ERRORLEVEL% EQU 0 GOTO Fin

7z a -r INF2705_remise_tp1.7z *.cpp *.h *.glsl makefile *.txt shaders && echo "remettre fichier INF2705_remise_tp1.7z fait avec 7z"
IF %ERRORLEVEL% EQU 0 GOTO Fin

rar a INF2705_remise_tp1.rar *.cpp *.h *.glsl makefile *.txt shaders && echo "remettre fichier INF2705_remise_tp1.tar fait avec rar"
IF %ERRORLEVEL% EQU 0 GOTO Fin

tar cvf INF2705_remise_tp1.tar *.cpp *.h *.glsl makefile *.txt shaders && echo "remettre fichier INF2705_remise_tp1.tar fait avec tar"
IF %ERRORLEVEL% EQU 0 GOTO Fin

echo "Ne sait pas comment faire INF2705_remise_tp1..."

:Fin
