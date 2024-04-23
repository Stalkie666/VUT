
class Point
{
private:
    
public:
    Point(double x, double y, double z, int id = 0);
    ~Point();
    void printPoint();
    double x,y,z;
    int id;
};

Point::Point(double x, double y, double z, int id)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->id = id;
}

Point::~Point()
{
}

void Point::printPoint(){
    std::cout << "( " << this->x << " ," << this->y << " ," << this->z <<")";
}