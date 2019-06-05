#include <iostream>
#include <stdlib.h>
#define BYTE unsigned char

using namespace std;


class Pixel
{
public:
    BYTE R,G,B;
public:
    Pixel() : R(0),G(0),B(0) {};
    Pixel(BYTE r, BYTE g, BYTE b) : R(r),G(g),B(b) {}
    Pixel (int value);
    int GrayScale() const;

    friend Pixel operator+(const Pixel &left, const Pixel &right);
    friend ostream& operator+(ostream &left, const Pixel &right);

    Pixel operator=(const Pixel &X);
};

Pixel::Pixel(int value)
{
    R=value%255;
    value/=255;
    G=value%255;
    value/=255;
    B=value%255;
}

int Pixel::GrayScale() const
{
    return((2*R+3*G+B)/8);
}

Pixel operator+(const Pixel &left, const Pixel &right)
{
    Pixel kq;
    kq.R=left.R+right.R;
    kq.G=left.G+right.G;
    kq.B=left.B+right.B;
    return(kq);
}

ostream& operator<<(ostream &left, const Pixel &right)
{
    left<<"("<<int(right.R)<<","<<int(right.G)<<","<<int(right.B)<<")"<<endl;
    return(left);
}

Pixel Pixel::operator=(const Pixel &X)
{
    R=X.R;
    G=X.G;
    B=X.B;
    return(*this);
}

//==============================
//==============================
//==============================
//==============================
//==============================

class Bitmap
{
    int width,height;
    Pixel **data;
    void freeData();
    void creatData(int w, int h);
public:
    ~Bitmap() {freeData();}
    Bitmap();
    Bitmap(int width, int height);
    Bitmap(const Bitmap &bmp);

    int getWidth() const;
    int getHeight() const;

    Bitmap Crop(int x, int y, int w, int h);

    int* Histogram();

    Pixel &operator()(int x, int y);
    Bitmap &operator=(const Bitmap &bmp);
};

void Bitmap::freeData()
{
    if (data==0) return;
    for (int i=0; i<height; i++)
        for (int j=0; j<width; j++)
            delete []data[i];
    delete []data;
}
void Bitmap::creatData(int w, int h)
{
    width=w; height=h;
    data=new Pixel*[height];
    for (int i=0; i<=height; i++)
        data[i]=new Pixel[width];
}

Bitmap::Bitmap()
{
    width=0;
    height=0;
    data=NULL;
}

Bitmap::Bitmap(int width, int height)
{
    creatData(width, height);
}

Bitmap::Bitmap(const Bitmap &bmp)
{
    creatData(bmp.width, bmp.height);
    for(int i=0; i<height; i++)
        for(int j=0; j<width;j++)
            data[i][j]=bmp.data[i][j];
}

int Bitmap::getHeight() const
{
    return height;
}


int Bitmap::getWidth() const
{
    return width;
}

Bitmap Bitmap::Crop(int x, int y, int w, int h)
{
    Bitmap T(w,h);
    for (int i=x; i<x+h; i++)
        for (int j=y; j<y+w; j++)
            T.data[i-x][j-y]=data[i][j];
    return (T);
}

int* Bitmap::Histogram()
{
    int *p=new int [256];
    for(int i=0; i<256; i++) p[i]=0;

    for(int i=0; i<height; i++)
        for(int j=0; j<width;j++)
            p[data[i][j].GrayScale()]++;
    return p;
}

Pixel& Bitmap::operator()(int x, int y)
{
    return data[x][y];
}

Bitmap& Bitmap::operator=(const Bitmap &bmp)
{
    freeData();
    creatData(bmp.getWidth(),bmp.getHeight());
     for(int i=0; i<height; i++)
        for(int j=0; j<width;j++)
            data[i][j]=bmp.data[i][j];

}


int main()
{
    Pixel p(0,60,120);
    cout<<"GrayScale: "<<p.GrayScale()<<endl;
    cout<<p+Pixel(10,10,10);

    Bitmap A;
    Bitmap B(100,100);
    for(int y=0; y<B.getHeight(); y++)
    {
        for (int x=0; x<B.getWidth(); x++)
            B(x,y)=Pixel(rand());
    }
    A=B;
    int *hist=A.Histogram();
    for(int i=0; i<256; i++)
        cout<<hist[i]<<" ";
    delete []hist;

    Bitmap C(B.Crop(20,20,40,60));

}
