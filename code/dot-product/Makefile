dotp_vanilla: dotp_flops.c
	gcc -O0 dotp_flops.c -o dotp_vanilla

dotp_opt: dotp_flops.c
	gcc -O1 dotp_flops.c -o dotp_opt 

dotp_vec: dotp_flops.c
	gcc -O3 -march=native -ftree-vectorize -funroll-loops -ffast-math -mavx2 -mfma dotp_flops.c -o dotp_vec

dotp_omp: dotp_flops_omp.c
	gcc -O3 -fopenmp dotp_flops_omp.c -o dotp_omp 

dotp_omp_vec: dotp_flops_omp.c
	gcc -O3 -fopenmp -march=native -ftree-vectorize -funroll-loops -ffast-math -mavx2 -mfma dotp_flops_omp.c -o dotp_omp_vec

clean:
	rm dotp_vanilla dotp_opt dotp_vec dotp_omp dotp_vec 


