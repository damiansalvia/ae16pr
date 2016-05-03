#include "prAE.hh"
#include <stdlib.h>

int main (int argc, char** argv)
{
	using skeleton prAE;

	system("clear");

	if(argc < 4)
		show_message(1);

	ifstream f1(argv[1]);
	if (!f1) show_message(11);

	ifstream f2(argv[2]);
	if (!f2) show_message(12);

	Problem pbm;
	f2 >> pbm;

	Operator_Pool pool(pbm);
	SetUpParams cfg(pool);
	f1 >> cfg;

	Solver_Seq solver(pbm,cfg);
	solver.run();

	if (solver.pid()==0)
	{
		solver.show_state();
		cout << "Solution: " << solver.global_best_solution()
		     << " Fitness: " << solver.global_best_solution().fitness() << endl;
		cout << "\n\n :( ---------------------- THE END --------------- :) ";

		// TODO - Borrar
		//Escribo el resultado en el archivo de salida
		FILE * pFile;
		pFile = fopen("res/solucion.out", "w");
		fprintf(pFile, "%d", solver.best_solution_trial().to_String());
		fclose(pFile);

		ofstream fexit(argv[3]);
		if(!fexit) show_message(13);
		fexit << solver.userstatistics();

	}
	return(0);
}
