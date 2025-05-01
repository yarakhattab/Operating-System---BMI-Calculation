//yara khattab 1210520
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <math.h>


//to store weight and height
struct ThePersoninformation
{
    double thepersonweight; // the person's weight
    double thepersonheight; // the person's height
};

// function to calculate bmi for each person
double BmiCalculate(double thepersonheight, double thepersonweight)
{
    double thepersonheight_convert=0.0;  // to covert the height from cm to m
    double BmiCalculate=0.0;// to calculate the bmi for persons
    thepersonheight_convert= (thepersonheight/100.0);// to covert the height from cm to m
    BmiCalculate= thepersonweight / (thepersonheight_convert * thepersonheight_convert) ;// to calculate the bmi for persons
    return BmiCalculate;

}



void ReadfromFile (int numOfpeopleinFile,struct ThePersoninformation* Theperson)
{
    // Read the data from the input file
    FILE* inputdatafile = fopen("bmi.csv", "r");
    if (inputdatafile == NULL)
    {
        perror("error in opening the file!!!!,try again");
        exit(EXIT_FAILURE);
    }

    //store the data from the input file file in the strucut
    for(int i = 0; i < numOfpeopleinFile; i++)
    {
        fscanf(inputdatafile, "%*[^,],%lf,%lf\n", &Theperson[i].thepersonheight, &Theperson[i].thepersonweight);

    }

    fclose(inputdatafile); //close the input file after finishing read the data
}


//******** Naive approach code ********

void Naive(int numOfpeopleinFile,struct ThePersoninformation* Theperson){
     // begin time
     struct  timespec BeginTime;
     timespec_get(&BeginTime,TIME_UTC);


    double SumOfallBMI = 0.0; //variable to store the total sum of bmi
    double Sumforperson=0.0;

    // for loop to calculate the sum bmi for each person
    for (int y = 0; y < numOfpeopleinFile; y++) {
        Sumforperson = BmiCalculate(Theperson[y].thepersonheight,Theperson[y].thepersonweight);
        SumOfallBMI = SumOfallBMI + Sumforperson;

    }

    //to find the avg bmi for all people in file
    double Avg_bmi = (SumOfallBMI /numOfpeopleinFile );

    printf("The Average BMI for Naive approach = %.2f\n", Avg_bmi); //to print the avg bmi using Naive approach

     // end time
     struct  timespec EndTime;
     timespec_get(&EndTime,TIME_UTC);

    double timespentinexecution=(EndTime.tv_sec - BeginTime.tv_sec)+(EndTime.tv_nsec - BeginTime.tv_nsec)/1000000000.0;// to get the execution time in seconds
    printf("The Execution Time using Naive approach = %lf second\n",timespentinexecution);//to print the execution time in seconds

}


//******** Multiprocessing approach code ********

void Multiprocessing (int numOfpeopleinFile,struct ThePersoninformation* Theperson)
{

     // begin time
    struct  timespec BeginTime;
    timespec_get(&BeginTime,TIME_UTC);


    int numberOfprocessneed = 4;
    double TheTotalBMI=0;
    double BMI =0.0;
    double BMIforeachprocess=0.0;
    double SumBMIprocess=0.0;
    int TheDataForeachprocess =numOfpeopleinFile/numberOfprocessneed;


    // Create pipes
    int fd[numberOfprocessneed][2]; // Four pipe file descriptors
    for (int i = 0; i < numberOfprocessneed; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            perror("Error, can not creat the pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < numberOfprocessneed; i++)
    {
        pid_t pid= fork();// creat new child
        if (pid == 0)
        {

            close(fd[i][0]); // Close read end
            for (int j = i * TheDataForeachprocess; j < (i + 1) *TheDataForeachprocess; j++) {
                 BMI = BmiCalculate(Theperson[j].thepersonheight, Theperson[j].thepersonweight);// calculate the bmi for each person
                 write(fd[i][1], &BMI, sizeof(double));
            }
            close(fd[i][1]);
            exit(EXIT_SUCCESS);

        }
        else if (pid < 0)
        {
            perror("Error forking process");
            exit(EXIT_FAILURE);
        }
    }

       // Parent process
        double SumOfallBMI = 0.0;
        for (int i = 0; i < numberOfprocessneed; i++) {
        close(fd[i][1]); // Close write end
        for (int j = i * TheDataForeachprocess; j < (i + 1) *TheDataForeachprocess; j++) {
            //double BMI;
            read(fd[i][0], &BMI, sizeof(double));
            SumOfallBMI += BMI;
        }
        close(fd[i][0]);
    }


    // Calculate and print the average BMI using Multiprocessing approach
    double averageBMI = SumOfallBMI / numOfpeopleinFile;
    printf(" The Average BMI for Multiprocessing approach: %.2lf\n", averageBMI);

    // parent Wait for all child processes to complete
    for (int i = 0; i < 4; ++i) {
        wait(NULL);
    }

    // end thime
    struct  timespec EndTime;
    timespec_get(&EndTime,TIME_UTC);

    double timespentinexecution=(EndTime.tv_sec - BeginTime.tv_sec)+(EndTime.tv_nsec - BeginTime.tv_nsec)/1000000000.0;// to get the execution time
    printf("The Execution Time using Multiprocessing approach = %lf second\n",timespentinexecution);//to print the execution time in seconds

}

//******** Multithreading  approach code ********

// Global variables for Multithreading
double totalSumBMIthread = 0.0; // store the sum of BMI for all persons
int numberofthreadsneed = 0; // To count the number of threads
pthread_mutex_t lock; // Mutex for protecting shared resources


// calculate BMI for thread
void *calculateBMIforthread(void *arg)
{
    FILE *file = (FILE *)arg;
    char persongender[10];
    double personheight, personweight;
    double bmiforoneperson;

    // Lock the mutex to read from the file
    pthread_mutex_lock(&lock);
    while (fscanf(file, "%[^,],%lf,%lf\n", persongender, &personheight, &personweight) == 3) {
        bmiforoneperson = BmiCalculate(personheight, personweight);
        totalSumBMIthread += bmiforoneperson;
        numberofthreadsneed++;
    }
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

void Multithreading (int numOfpeopleinFile,struct ThePersoninformation* Theperson){


     // begin time
    struct  timespec BeginTime;
    timespec_get(&BeginTime,TIME_UTC);


    int number_of_thread =4;
    if (pthread_mutex_init(&lock, NULL) != 0) // Initialize the mutex
    {
        printf("Mutex init has failed\n");
        return 1;
    }

    FILE *file = fopen("bmi.csv", "r"); // Open the file
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    pthread_t threads[number_of_thread];// Create threads
    for (int m = 0; m < number_of_thread; m++)
    {
        if (pthread_create(&threads[m], NULL, calculateBMIforthread, (void *)file))
        {
            perror("Error in creating thread");
            return 1;
        }
    }
    for (int m = 0; m < number_of_thread; m++) // Join threads
    {
        pthread_join(threads[m], NULL);
    }

    fclose(file); // Close the file
    pthread_mutex_destroy(&lock);// Destroy the mutex

    if (numberofthreadsneed > 0)  // Calculate average BMI
    {
        printf("The Average BMI for Multithreading approach:: %.2lf\n", totalSumBMIthread / numberofthreadsneed);
    }
    else
    {
        printf("No data found.\n");
    }

     // end thime
    struct  timespec EndTime;
    timespec_get(&EndTime,TIME_UTC);

    double timespentinexecution=(EndTime.tv_sec - BeginTime.tv_sec)+(EndTime.tv_nsec - BeginTime.tv_nsec)/1000000000.0;// to get the execution time
    printf("The Execution Time using Multithreading approach = %lf second\n",timespentinexecution);//to print the execution time in seconds


}


int main()
{
    int numOfpeopleinFile=500;

    struct ThePersoninformation person [numOfpeopleinFile];
    ReadfromFile(numOfpeopleinFile,person);//read and return number of person

    Naive(numOfpeopleinFile,person);
    Multiprocessing(numOfpeopleinFile,person);
    Multithreading(numOfpeopleinFile,person);



    return 0;
}

