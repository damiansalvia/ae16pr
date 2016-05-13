#include "prAE.hh"
#include <stdlib.h>
#include <string.h>

int main (int argc, char** argv)
{
	using skeleton prAE;

	system("clear");

	// Verificar que estén todos los argumentos
	// self cgf num tas nom out
	if(argc < 6) show_message(1);

	// Cargar *.cfg
	ifstream f1(argv[1]);
	if (!f1) show_message(11);

	// Crear archivo con parametros
	char path[32]; // <nomb>/<nomb>.in
	sprintf((char*)path,"ins/%s.in",argv[4]);
	FILE *fp = fopen(path,"w");
	fprintf(fp,"%s\n%s\n%s\n",argv[2],argv[3],argv[4]);
	fclose(fp);

	// Cargar archivo con parametros
	ifstream f2(path);
	if (!f2) show_message(12);

	// Cargar problema
	Problem pbm;
	f2 >> pbm;

	// Cargar parametros de configuracion
	Operator_Pool pool(pbm);
	SetUpParams cfg(pool);
	f1 >> cfg;

	// Arrancar algoritmo
	Solver_Seq solver(pbm,cfg);
	solver.run();

	if (solver.pid()==0)
	{
		// Mostrar estadísticas finales
		solver.show_state();
		cout << "Solution: " << solver.global_best_solution()
		     << " Fitness: " << round(solver.global_best_solution().fitness()) // FIXME
			 << " Iteration: " << solver.iteration_best_found()<< endl;
		int ms = ((int)solver.current_time_spent() % 1000),
			s  = ((int)(solver.current_time_spent() / 1000) % 60),
			m  = ((int)(solver.current_time_spent() / (1000*60)) % 60);
		cout << "Time elapsed: "
				<< ((m<10)?"0":"") << m << ":"
				<< ((s<10)?"0":"") << s << "'"
				<< ms << endl;
		cout << "Iteraion best foundx: " << solver.iteration_best_found() << endl;
		cout << "\n\n :( ---------------------- THE END --------------- :) " <<endl;

		// Copiar solucion al archivo de salida
		ofstream fexit(argv[5]);
		if(!fexit) show_message(13);
		fexit << solver.best_solution_trial();

	}
	return(0);
}
