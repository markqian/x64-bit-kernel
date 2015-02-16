int a;
char b = 'k';
void main() {
  a = 'B';
  char* video_memory = (char*) 0xb8000;
  *video_memory = a;
  
}
