#include "Monitoring.h"
 
Monitoring::Monitoring(){

	isBreak = false;
}

void Monitoring::init(){
        FILE* file;
        struct tms timeSample;
        char line[128];
    

        lastCPU = times(&timeSample);
        lastSysCPU = timeSample.tms_stime;
        lastUserCPU = timeSample.tms_utime;
    
        file = fopen("/proc/cpuinfo", "r");
        numProcessors = 0;
        while(fgets(line, 128, file) != NULL){
            if (strncmp(line, "processor", 9) == 0) numProcessors++;
        }
        //cout<<"TOTAL NO OF PROCESSOR :"<<numProcessors<<endl;
        fclose(file);

        file = fopen("/proc/stat", "r");
        fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow, &lastTotalSys, &lastTotalIdle);
        fclose(file);
}
    

double Monitoring::getProcessCurrentCpuUtilisation(){
        struct tms timeSample;
        clock_t now;
        double percent;
    

        now = times(&timeSample);
        if (now <= lastCPU || timeSample.tms_stime < lastSysCPU ||
            timeSample.tms_utime < lastUserCPU){
            //Overflow detection. Just skip this value.
            percent = -1.0;
        }
        else{
            percent = (timeSample.tms_stime - lastSysCPU) +
                (timeSample.tms_utime - lastUserCPU);
            percent /= (now - lastCPU);
            percent /= numProcessors;
            percent *= 100;
        }
        lastCPU = now;
        lastSysCPU = timeSample.tms_stime;
        lastUserCPU = timeSample.tms_utime;
    

        return percent;
}


double Monitoring::getCurrentCpuUtilisation(){
       double percent;
       FILE* file;
       unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;


       file = fopen("/proc/stat", "r");
       fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
           &totalSys, &totalIdle);
       fclose(file);


       if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
           totalSys < lastTotalSys || totalIdle < lastTotalIdle){
           //Overflow detection. Just skip this value.
           percent = -1.0;
       }
       else{
           total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
               (totalSys - lastTotalSys);
           percent = total;
           total += (totalIdle - lastTotalIdle);
           percent /= total;
           percent *= 100;
       }

       lastTotalUser = totalUser;
       lastTotalUserLow = totalUserLow;
       lastTotalSys = totalSys;
       lastTotalIdle = totalIdle;

       return percent;
}


void Monitoring::setMemoryInfo(){

	struct sysinfo memInfo;
    sysinfo (&memInfo);
    long long totalVirtualMem = memInfo.totalram;
    //Add other values in next statement to avoid int overflow on right hand side...
    totalVirtualMem += memInfo.totalswap;
    totalVirtualMem *= memInfo.mem_unit;

    long long virtualMemUsed = memInfo.totalram - memInfo.freeram;
        //Add other values in next statement to avoid int overflow on right hand side...
    virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
    virtualMemUsed *= memInfo.mem_unit;

    long long totalPhysMem = memInfo.totalram;
        //Multiply in next statement to avoid int overflow on right hand side...
    totalPhysMem *= memInfo.mem_unit;

    long long physMemUsed = memInfo.totalram - memInfo.freeram;
        //Multiply in next statement to avoid int overflow on right hand side...
    physMemUsed *= memInfo.mem_unit;

}

int parseLine(char* line){
        int i = strlen(line);
        while (*line < '0' || *line > '9') line++;
        line[i-3] = '\0';
        i = atoi(line);
        return i;
    }


 int Monitoring::getProcessCurrentVirtualMemoryUtilisation(){ //Note: this value is in KB!
        FILE* file = fopen("/proc/self/status", "r");
        int result = -1;
        char line[128];


        while (fgets(line, 128, file) != NULL){
            if (strncmp(line, "VmSize:", 7) == 0){
                result = parseLine(line);
                break;
            }
        }
        fclose(file);
        return result;
    }


 int Monitoring::getProcessCurrentPhysicalMemoryUtilisation(){ //Note: this value is in KB!
     FILE* file = fopen("/proc/self/status", "r");
     int result = -1;
     char line[128];


     while (fgets(line, 128, file) != NULL){
         if (strncmp(line, "VmRSS:", 6) == 0){
             result = parseLine(line);
             break;
         }
     }
     fclose(file);
     return result;
 }

 int  Monitoring::parse_netdev(unsigned long long int *receivedabs, unsigned long long int *sentabs)
 {
 	char buf[255];
 	char *datastart;
 	static int bufsize;
 	int rval;
 	FILE *devfd;
 	unsigned long long int receivedacc, sentacc;

 	bufsize = 255;
 	devfd = fopen("/proc/net/dev", "r");
 	rval = 1;

 	// Ignore the first two lines of the file
 	fgets(buf, bufsize, devfd);
 	fgets(buf, bufsize, devfd);

 	while (fgets(buf, bufsize, devfd)) {
 	    if ((datastart = strstr(buf, "lo:")) == NULL) {
 		datastart = strstr(buf, ":");

 		sscanf(datastart + 1, "%llu  %*d     %*d  %*d  %*d  %*d   %*d        %*d       %llu",\
 		       &receivedacc, &sentacc);
 		*receivedabs += receivedacc;
 		*sentabs += sentacc;
 		rval = 0;
 	    }
 	}

 	fclose(devfd);
 	return rval;
 }

 void Monitoring::calculate_speed(char *speedstr, unsigned long long int newval, unsigned long long int oldval)
 {
 	double speed;
 	speed = (newval - oldval) / 1024.0;
 	if (speed > 1024.0) {
 	    speed /= 1024.0;
 	    sprintf(speedstr, "%.3f MB/s", speed);
 	} else {
 	    sprintf(speedstr, "%.2f KB/s", speed);
 	}
 }

 char* Monitoring::get_netusage(unsigned long long int *rec, unsigned long long int *sent)
 {
 	unsigned long long int newrec, newsent;
 	newrec = newsent = 0;
 	char downspeedstr[15], upspeedstr[15];
 	static char retstr[42];
 	int retval;

 	retval = parse_netdev(&newrec, &newsent);
 	if (retval) {
 	    fprintf(stdout, "Error when parsing /proc/net/dev file.\n");
 	    exit(1);
 	}

 	calculate_speed(downspeedstr, newrec, *rec);
 	calculate_speed(upspeedstr, newsent, *sent);

 	sprintf(retstr, "down: %s up: %s", downspeedstr, upspeedstr);

 	*rec = newrec;
 	*sent = newsent;
 	return retstr;
 }

 void Monitoring::ThreadFunc()
 {
	 init();
	 static unsigned long long int rec, sent;
	 double ProcessCpuUtilisation, OverallCpuUtilisation;
	 int ProcessVirtualMemoryUtilisation, ProcessPhysicalMemoryUtilisation;
	 unsigned long long int DataSent,DataReceived;

	 while(1){
		 if ( isBreak ){
			 break;
		 }
		 sleep(10);
		// cout<<"\nCreating Monitoring DBA\n";
		 dBA = new DataBaseAccess();
	 	 ProcessCpuUtilisation = getProcessCurrentCpuUtilisation();
	 	 OverallCpuUtilisation = getCurrentCpuUtilisation();
	 	 ProcessVirtualMemoryUtilisation = getProcessCurrentVirtualMemoryUtilisation();
	 	 ProcessPhysicalMemoryUtilisation = getProcessCurrentPhysicalMemoryUtilisation();
	 	 get_netusage(&DataSent, &DataReceived);
	 	 dBA->InsertIntoProcessMonitor(ProcessCpuUtilisation,OverallCpuUtilisation,ProcessVirtualMemoryUtilisation,ProcessPhysicalMemoryUtilisation,DataSent, DataReceived);
	 	 dBA->Close();
	 	 delete dBA;
	 	//cout<<"\nClose Monitoring DBA\n";
	 }

 }
