int main ()
{
  char* arr[] = {"exit",(char*)0};
  execvp("exit",arr);
  printf ("hello");
}
