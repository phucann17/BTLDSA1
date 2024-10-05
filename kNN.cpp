#include "kNN.hpp"

/* TODO: You can implement methods, functions that support your data structures here.
 * */

template <class T> class LinkedList : public List<T> {
public:
  class Node {
  private:
    T data;
    Node *next;
    friend class LinkedList<T>;

  public:
    Node() { next = nullptr; }
    Node(Node *next) { this->next = next; }
    Node(T data, Node *next = nullptr) {
      this->data = data;
      this->next = next;
    }
  };

protected:
  Node *head;
  Node *tail;
  int count;

public:
  LinkedList() {
    head = nullptr;
    tail = nullptr;
    count = 0;
  }
  ~LinkedList(){this->clear();}
  void push_back(T value) override {
    Node *tmp = new Node(value, nullptr);
    if (this->count == 0) {
      this->head = this->tail = tmp;
    } else {
      this->tail->next = tmp;
      this->tail = tmp;
    }
    this->count++;
  }

  void push_front(T value) override {
    Node *tmp = new Node(value, nullptr);
    if (this->count == 0) {
      this->head = this->tail = tmp;
    } else {
      tmp->next = this->head;
      this->head = tmp;
    }
    this->count++;
  }

  void insert(int index, T value) override {
    if (index < 0 || index > count)
      return;
    else if (index == count)
      push_back(value);
    else if (index == 0)
      push_front(value);
    else {
      Node *pre = this->head;
      for (int i = 0; i < index - 1; i++) {
        pre = pre->next;
      }
      Node *tmp = new Node(value, pre->next);
      pre->next = tmp;
      this->count++;
    }
  }

  int length() const override { return this->count; }

  void print() const override {
    if (this->count == 0) {
      return;
    } else {
      Node *tmp = head;
      cout << tmp->data;
      for (int i = 1; i < this->count; i++) {
        tmp = tmp->next;
        cout << " " << tmp->data;
      }
    }
  }

  void remove(int index) override {
    if (index < 0 || index >= count)return;
    if (count == 1) {
      delete this->head;
      this->head = nullptr;
      this->tail = nullptr;
    } else {
      Node *tmp = this->head;
      if (index == 0) {
          this->head = head->next;
          delete tmp;
      } else if (index == this->count - 1) {
          for (int i = 0; i < count - 2; i++) {
              tmp = tmp->next;
          }
          delete tmp->next;
          tmp->next = nullptr;
          this->tail = tmp;
      } else {
          for (int i = 0; i < index - 1; i++) {
              tmp = tmp->next;
          }
          Node *tmp2 = tmp->next;
          tmp->next = tmp2->next;
          delete tmp2;
      }
    }
    --this->count;
  }

  void clear() override {
    while (this->count != 0)
      remove(0);
  }

  void reverse() override {
    if (this->count == 0) {
      return;
    } else {
      Node *pre = nullptr;
      Node *next = nullptr;
      Node *tmp = this->head;
      while (tmp != nullptr) {
        next = tmp->next;
        tmp->next = pre;
        pre = tmp;
        tmp = next;
      }
      this->tail = this->head;
      this->head = pre;
    }
  }

  T &get(int index) const override {
    if (index < 0 || index >= this->count)
      throw std::out_of_range("get(): Out of range");
    else {
      Node *temp = this->head;
      for (int i = 0; i < index; i++) {
        temp = temp->next;
      }
      return temp->data;
    }
  }
List<T>* operator=(const List<T> *other) override {
  this->clear();
  int sz = other->length();
  for (int i = 0; i < sz; ++i) {
    this->push_back(other->get(i));
  }
  return this;
}

};

void swap(List<double> *list1, List<double> *list2) {
  List<double> *temp = new LinkedList<double>();
  int sz1 = list1->length();
  for (int i = 0; i < sz1; ++i) {
    temp->push_back(list1->get(i));
  }
  list1->clear();
  int sz2 = list2->length();
  for (int i = 0; i < sz2; ++i) {
    list1->push_back(list2->get(i));
  }
  list2->clear();
  int sz3 = temp->length();
  for (int i = 0; i < sz3; ++i){
    list2->push_back(temp->get(i));
  }
}

 void bbsort(List<List<double> *> *list, int sz) {
    for (int i = 0; i < sz - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < sz - i - 1; ++j) {
            List<double> *current = list->get(j);
            List<double> *next = list->get(j + 1);
            if (current->get(1) > next->get(1)) {
                swap(current, next);
                swapped = true;
            }
        }
        if (!swapped) break; // Nếu không có sự hoán đổi, dãy đã sắp xếp
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////
Dataset::Dataset (){ data = new LinkedList<List<int> *>();
  title = new LinkedList<List<string> *>(); }
bool Dataset::loadFromCSV(const char* fileName){
  ifstream file(fileName);
  if (!file.is_open()) {
      return false;
  }
  string line;
  if (getline(file, line)) {
      stringstream ss(line);
      List<string> *titleRow = new LinkedList<string>();
      string cell;
      while (getline(ss, cell, ',')) {
          titleRow->push_back(cell);
      }
      title->push_back(titleRow);
  }
  while (getline(file, line)) {
      stringstream ss(line);
      List<int> *dataRow = new LinkedList<int>();
      string cell;
      while (getline(ss, cell, ',')) {
          int value = stoi(cell);
          dataRow->push_back(value);
      }
      data->push_back(dataRow);
  }

  file.close();
  return true;
}
////////////////////////////////////////////////////
void Dataset::printHead(int nRows, int nCols) const{
  if (nRows <= 0 || nCols <= 0) {
    return;
  }

  // Print title
  int totalTitleCols = (nCols < title->get(0)->length()) ? nCols : title->get(0)->length();
  cout << title->get(0)->get(0);
  for (int j = 1; j < totalTitleCols; j++) {
    cout << " " << title->get(0)->get(j);
  }

  // Print data
  int totalCols = (nCols < data->get(0)->length()) ? nCols : data->get(0)->length();
  int totalRows = (nRows < data->length()) ? nRows : data->length();
  for (int i = 0; i < totalRows; i++) {
    List<int>* row = data->get(i);
    cout << endl;
    cout << row->get(0);
    for (int j = 1; j < totalCols; j++) {
      cout << " " << row->get(j);
    }
  }
}
////////////////////////////////////////////////////
void Dataset::printTail(int nRows, int nCols) const{
  if (nRows <= 0 || nCols <= 0) {
      return;
  }

  int totalCols = (nCols < data->get(0)->length()) ? nCols : data->get(0)->length();
  int totalRows = (nRows < data->length()) ? nRows : data->length();
  //Print title
  cout << title->get(0)->get(title->get(0)->length()-totalCols);
  for (int j = title->get(0)->length()-totalCols + 1; j < title->get(0)->length(); j++) {
    cout << " " << title->get(0)->get(j);
  }
  //Print data
  for (int i = data->length() - totalRows; i < data->length(); i++) {
    cout << endl;
    cout<<data->get(i)->get(data->get(data->length() - 1)->length() - totalCols);
      for (int j = data->get(i)->length()+ 1 - totalCols ; j < data->get(i)->length(); j++) {
          cout << " " <<data->get(i)->get(j) ;
      }
  }
}
////////////////////////////////////////////////////
void Dataset::getShape(int& nRows, int& nCols) const{
  if(this->data->length()<0) {return;}
  else {nRows = data->length();
  if (nRows > 0) {
      nCols = data->get(0)->length();
  } else  {
      nCols = 0;
  }
}}
////////////////////////////////////////////////////
void Dataset::columns() const{
  cout <<  title->get(0)->get(0);
  for (int j = 1; j < title->get(0)->length(); j++) {
    cout <<" " <<  title->get(0)->get(j);
  }
}

////////////////////////////////////////////////////
bool Dataset::drop(int axis, int index, string columns){
  bool found = false;
  if(title->get(0)->length()<=0) found= false;
  if (axis == 0 && this->data->length() >= 0) { 
      if (index >= data->length() || index < 0) {found= false;}
      else{
         found= true;
      data->remove(index);
     }
  } else if(axis == 1 && this->data->length() > 0) {
      for (int i = 0; i < title->get(0)->length(); i++) {
          if ( columns==title->get(0)->get(i)) {
              found = true;
              title->get(0)->remove(i);
              for (int j = 0; j < data->length(); j++) {
                  List<int> *row = data->get(j);
                  row->remove(i); 
              }
            break;
          }
      }
  }
  else{found=false;}
  return found;
}
////////////////////////////////////////////////////
Dataset Dataset::extract(int startRow, int endRow, int startCol, int endCol) const{
  Dataset extractedDataset;
  if (endRow == -1 || endRow>=data->length() ) {endRow = data->length()-1;}
  if (endCol == -1 || endCol>=data->get(0)->length()) {endCol = data->get(0)->length()-1;}
  if ( this->data->length() < 0 || this->data->get(0)->length() < 0 || startRow > endRow || startCol > endCol || startCol<0 || startRow<0 || endCol<-1 || endRow<-1) {
      // return extractedDataset;
      throw std::out_of_range("get(): Out of range");
  }
  else{
   List<string> *exTitle = new LinkedList<string>();
    for (int i = startCol; i <= endCol; i++) {
      exTitle->push_back(title->get(0)->get(i));
    }
    extractedDataset.title->push_back(exTitle);
    for (int i = startRow; i <= endRow; ++i) {
      List<int> *row = data->get(i);
      List<int> *extractedRow = new LinkedList<int>();
      for (int j = startCol; j <= endCol; ++j) {
        extractedRow->push_back(row->get(j));
      }
      extractedDataset.data->push_back(extractedRow);
    }
  return extractedDataset;}
}
////////////////////////////////////////////////////
List<List<int>*>* Dataset::getData() const{return this->data;}
List<List<string>*>* Dataset::getTitle() const{return this->title;}
Dataset ::Dataset(const Dataset &other){
  data = new LinkedList<List<int>*>();
  title = new LinkedList<List<string>*>();
  // Copy data
  for (int i = 0; i < other.data->length(); ++i) {
      List<int>* newRow = new LinkedList<int>();
      List<int>* otherRow = other.data->get(i);
      for (int j = 0; j < otherRow->length(); ++j) {
          newRow->push_back(otherRow->get(j));
      }
      data->push_back(newRow);
  }
  // Copy title
  for (int i = 0; i < other.title->length(); ++i) {
      List<string>* newTitleRow = new LinkedList<string>();
      List<string>* otherTitleRow = other.title->get(i);
      for (int j = 0; j < otherTitleRow->length(); ++j) {
          newTitleRow->push_back(otherTitleRow->get(j));
      }
      title->push_back(newTitleRow);
  }
}

Dataset& Dataset::operator=(const Dataset& other) {
    if (this == &other) {
        return *this;
    }

    // Clear existing data and title
    data->clear();
    title->clear();

    // Copy data
    for (int i = 0; i < other.data->length(); ++i) {
        List<int>* newRow = new LinkedList<int>();
        List<int>* otherRow = other.data->get(i);
        for (int j = 0; j < otherRow->length(); ++j) {
            newRow->push_back(otherRow->get(j));
        }
        data->push_back(newRow);
    }

    // Copy title
    for (int i = 0; i < other.title->length(); ++i) {
        List<string>* newTitleRow = new LinkedList<string>();
        List<string>* otherTitleRow = other.title->get(i);
        for (int j = 0; j < otherTitleRow->length(); ++j) {
            newTitleRow->push_back(otherTitleRow->get(j));
        }
        title->push_back(newTitleRow);
    }

    return *this;
}


//////////////////////////////////////////////////////////////////////////////////////////////
void kNN:: fit(const Dataset &X_train, const Dataset &y_train){
  this->X_train = X_train;
  this->y_train = y_train;
}

Dataset kNN::predict(const Dataset &X_test){
  Dataset ypred;
  if (X_train.getData()->length() <= 0 || y_train.getData()->length() <= 0 || k <= 0 || k > X_train.getData()->length()) {
      return ypred;
  }

  int nRowsY_train = y_train.getData()->length();
  int nRowsX_train = X_train.getData()->length();
  int nColsX_train = X_train.getData()->get(0)->length();

  // Sao chép tiêu đề
  for (int i = 0; i < y_train.getTitle()->length(); ++i) {
      List<string> *newTitleRow = new LinkedList<string>();
      List<string> *yTitleRow = y_train.getTitle()->get(i);
      for (int j = 0; j < yTitleRow->length(); ++j) {
          newTitleRow->push_back(yTitleRow->get(j));
      }
      ypred.getTitle()->push_back(newTitleRow);
  }

  // Tính toán khoảng cách và sắp xếp
  for (int i = 0; i < X_test.getData()->length(); ++i) {
      List<int> *x_testrow = X_test.getData()->get(i);
      double *distance = new double[nRowsX_train];
      for (int j = 0; j < nRowsX_train; ++j) {
          List<int> *x_trainrow = X_train.getData()->get(j);
          double dist = 0.0;
          for (int k = 0; k < nColsX_train; ++k) {
              double diff = static_cast<double>(x_testrow->get(k) - x_trainrow->get(k));
              dist += diff * diff;
          }
          distance[j] = dist;
      }

      List<List<double> *> *label_distance = new LinkedList<List<double> *>();
      for (int j = 0; j < nRowsX_train; ++j) {
          List<double> *newRow = new LinkedList<double>();
          newRow->push_back(y_train.getData()->get(j)->get(0));
          newRow->push_back(distance[j]);
          label_distance->push_back(newRow);
      }
      delete[] distance; // Giải phóng bộ nhớ

      bbsort(label_distance, nRowsX_train);

      // Phân loại
      int cnt[10] = {0};
      for (int j = 0; j < k; ++j) {
          cnt[static_cast<int>(label_distance->get(j)->get(0))]++;
      }
      int max_freq = -1, value = 10;
      for (int j = 0; j < 10; ++j) {
          if (cnt[j] > max_freq) {
              max_freq = cnt[j];
              value = j;
          } else if (cnt[j] == max_freq) {
              value = min(value, j);
          }
      }

      LinkedList<int> *res = new LinkedList<int>();
      res->push_back(value);
      ypred.getData()->push_back(res);
  }

  return ypred;
  }


double kNN::score(const Dataset& y_test, const Dataset& y_pred){
  if(y_pred.getData()->length() <= 0 || y_test.getData()->length() <= 0) throw std::out_of_range("get(): Out of range");
    if(y_pred.getData()->length() != y_test.getData()->length() ||
       y_pred.getData()->get(0)->length() != 1 ||
       y_test.getData()->get(0)->length() != 1) 
    {
        throw std::out_of_range("get(): Out of range");
    }
    int nTest = y_test.getData()->length();
    int cnt = 0;
    
    for (int i = 0; i < nTest; ++i) {
        if (y_test.getData()->get(i)->get(0) == y_pred.getData()->get(i)->get(0)) {
            cnt++;
        }
    }
    double accuracy = static_cast<double>(cnt) / nTest;
    return accuracy;
}

void train_test_split(Dataset& X, Dataset& y, double test_size, 
                      Dataset& X_train, Dataset& X_test, Dataset& y_train, Dataset& y_test){
if (X.getData()->length() != y.getData()->length() || test_size >= 1 || test_size <= 0)
 return;
  int nRow = X.getData()->length();
 double minDoubledistance = 1.0e-15;
double rowsplit = nRow * (1 - test_size);

 if (abs(round(rowsplit) - rowsplit) < minDoubledistance * nRow)
 rowsplit = round(rowsplit);
 X_train = X.extract(0, rowsplit - 1, 0, -1);
 y_train = y.extract(0, rowsplit - 1, 0, -1);
 X_test = X.extract(rowsplit, -1, 0, -1);
 y_test = y.extract(rowsplit, -1, 0, -1);
}