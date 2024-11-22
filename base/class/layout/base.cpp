class Pepole {
 public:
  int i;
  Pepole(int ii) { i = ii; }
};

class Student : public Pepole {
 public:
  int j;
  Student(int ii, int jj) : Pepole{ii} { j = jj; }
};

int main() {
  Student(11, 22);
  return 0;
}
