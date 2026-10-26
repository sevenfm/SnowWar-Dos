// Generate error report file

void ErrorReport(char *String){
  FILE *ErrorFile;
  ErrorFile=fopen("error.txt","a");
  fprintf(ErrorFile,"\n%s",String);
  fclose(ErrorFile);
}

void InitErrorReport(void){
  FILE *ErrorFile;
  ErrorFile=fopen("error.txt","w");
  fprintf(ErrorFile,"Error report:");
  fclose(ErrorFile);
}
