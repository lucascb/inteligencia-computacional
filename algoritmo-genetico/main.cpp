#include "genetico.h"

int nfilhos = (PCROSS / 100.0) * PMAX;
int nmut    = (PMUT   / 100.0) * PMAX;
int elite   = (ELITE  / 100.0) * PMAX;

int main(int argc, char *argv[]) {
	ind_t pop[PMAX+nfilhos], filhos[nfilhos], ind1, ind2, result[2];
	int i, ger, cont;
	//double medias_inicial[50][6], medias_final[50][6];
	//printf("%d\n", RAND_MAX);

	clock_t start = clock();
	srand(time(NULL));

	cont = 0;
	for (int c = 0; c < 1000; c++) {
		/* Gera PMAX individuos aleatorios para a populacao inicial
		for (i = 0; i < PMAX; i++) {
			pop[i] = gera_individuo();
		}
		//*/
		povoar(pop);
		//printf("\tPopulacao inicial\n\n");
		//analisa_populacao(pop, medias_inicial[c]);

		for (ger = 0; !convergiu(pop) && ger < NGER; ger++) {
			monta_roleta(pop);
			// Seleciona nfilhos individuos aleatorios para o crossover
			for (i = 0; i < nfilhos; i += 2) {
				//ind1 = torneio(pop);
				//ind2 = torneio(pop);
				ind1 = roleta(pop);
				do { ind2 = roleta(pop); } while (igual(ind1, ind2));
				//crossover_ciclico(ind1, ind2, result);
				crossover_pmx(ind1, ind2, result);
				filhos[i]   = result[0];
				filhos[i+1] = result[1];
				/* Testes
				if (!valido(filhos[i]) || !valido(filhos[i+1])) {
					printf("*** Filho invalido ***\n");
					return 0;
				}
				//*/
			}

			// Seleciona PMUT filhos aleatorios e os mutacionam
			for (int m = 0; m < nmut; m++) {
				i = randint(PCROSS);
				mutaciona(&filhos[i]);
			}

			/* Reinsercao Ordenada: Ordena os individuos e pega os PMAX melhores
			for (i = 0; i < nfilhos; i++)
				pop[i+PMAX] = filhos[i];
			std::sort(pop, pop+PMAX+nfilhos, mais_apto);
			//*/

			//* Elitismo: Mantem uma porcentagem dos melhores pais na populacao
			std::sort(pop, pop+PMAX, mais_apto);
            for (i = 0; i < nfilhos; i++)
                pop[i + elite] = filhos[i];
			//*/

			//printf("\n\tPopulacao gerada\n\n");
			//analisa_populacao(pop, medias_final[c]);
		}
		//printf("Resultado fit: %d\n", pop[0].fit);
		if (convergiu(pop)) {
			//if (cont == 0) print(pop[0]);
			cont++;
		}
	}

	clock_t end = clock();
	printf("Taxa de convergencia: %.2f%%\n", (cont / 1000.0) * 100);
	printf("Tempo de execucao: %fs\n", (float) (end - start) / CLOCKS_PER_SEC);
	/* Testes
	double fit_pop_in, n_0_in, n_1_in, n_100_in, fit_100_in, melhor_ind_in;
	double fit_pop_fin, n_0_fin, n_1_fin, n_100_fin, fit_100_fin, melhor_ind_fin;

	fit_pop_in = n_0_in = n_1_in = n_100_in = fit_100_in = melhor_ind_in = 0;
	fit_pop_fin = n_0_fin = n_1_fin = n_100_fin = fit_100_fin = melhor_ind_fin = 0;
	for (int c = 0; c < 50; c++) {
		// Medias populacao inicial
		fit_pop_in    += medias_inicial[c][0];
		melhor_ind_in += medias_inicial[c][1];
		n_100_in 	  += medias_inicial[c][2];
		fit_100_in    += medias_inicial[c][3];
		n_0_in 		  += medias_inicial[c][4];
		n_1_in 		  += medias_inicial[c][5];
		// Medias populacao final
		fit_pop_fin    += medias_final[c][0];
		melhor_ind_fin += medias_final[c][1];
		n_100_fin      += medias_final[c][2];
		fit_100_fin    += medias_final[c][3];
		n_0_fin        += medias_final[c][4];
		n_1_fin        += medias_final[c][5];
	}
	printf("\tMedias populacao inicial\n\n");
	printf("Media fitness: \t\t%.2lf\n",   fit_pop_in / 50.0);
	printf("Melhor fitness: \t%.2lf\n", melhor_ind_in / 50.0);
	printf("Nro de ind < 100: \t%.2lf\n", 	 n_100_in / 50.0);
	printf("Media ind < 100: \t%.2lf\n",   fit_100_in / 50.0);
	printf("Nro de ind com M = 0: \t%.2lf\n",  n_0_in / 50.0);
	printf("Nro de ind com M = 1: \t%.2lf\n",  n_1_in / 50.0);

	printf("\n\tMedias populacao gerada\n\n");
	printf("Media fitness: \t\t%.2lf\n",   fit_pop_fin / 50.0);
	printf("Melhor fitness: \t%.2lf\n", melhor_ind_fin / 50.0);
	printf("Nro de ind < 100: \t%.2lf\n",    n_100_fin / 50.0);
	printf("Media ind < 100: \t%.2lf\n",   fit_100_fin / 50.0);
	printf("Nro de ind com M = 0: \t%.2lf\n",  n_0_fin / 50.0);
	printf("Nro de ind com M = 1: \t%.2lf\n",  n_1_fin / 50.0);
	//*/

	return 0;
}
