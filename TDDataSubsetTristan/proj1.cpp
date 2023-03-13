

#include "exo-vtk-include.h"
#include "helpers.h"
#include "config.h"

#define STR_VALUE(arg)      #arg
#define FUNCTION_NAME(name) STR_VALUE(name)
#define TAILLE_NAME FUNCTION_NAME(TAILLE)

//ICI PLACER LA TAILLE DU FICHIER 512 ou 1024
#define TAILLE 512
//#define TAILLE 1024

#define FICHIER MY_MESHES_PATH "/sn_resamp" TAILLE_NAME


int gridSize = TAILLE;
const char *location = FICHIER;
using std::cerr;
using std::endl;


// Function prototypes
vtkRectilinearGrid  *ReadGrid(int zStart, int zEnd);

bool intersectPlane(const double *pDouble, const double *pDouble1, double d);

bool PositiveSideOfPlane(const double *origin, const double *normal,
                         const double *point)
{
    double sum = normal[0]*(origin[0]-point[0])
                 + normal[1]*(origin[1]-point[1])
                 + normal[2]*(origin[2]-point[2]);
    return (sum < 0.);
}


void
GetRelevantZRange(const double *origin, const double *normal, int *range)
{
    // The data goes from 0 to 1 in X, Y, and Z.
    // There are gridSize points in each direction.
    // The i^{th} z-plane is located at i/(gridSize-1.0).
    // range[0] = 0;
    // range[1] = gridSize*0.5-1;
    // range[1] = gridSize-1;

    range[0] = -1;
    range[1] = -1;

    for(int i=0;i<gridSize;i++)
    {
        if ( intersectPlane(origin, normal,(double)i) && (range[0]==-1))
        {
            range[0]=i;

        }

        if( !intersectPlane(origin, normal,(double)i) && (range[1]==-1) && (range[0]!=-1)){
            range[1] = i;
        }

        //if (range[0]==-1) range[0]=0;

        //if (range[1]==-1) range[1]=-1;

    }
}

bool intersectPlane(const double *origin, const double *normal,
                    const double slice) {
    double point[3];
    point[0] = 0;
    point[1] = 0;
    point[2] = slice / (gridSize - 1.0);

    bool side = PositiveSideOfPlane(origin, normal, point);


    point[1] = 1;

    if (side != PositiveSideOfPlane(origin, normal, point))
        return true;

    point[0] = 1;
    point[1] = 0;

    if (side != PositiveSideOfPlane(origin, normal, point))
        return true;

    point[0] = 1;
    point[1] = 1;

    if (side != PositiveSideOfPlane(origin, normal, point))
        return true;

    return false;
}


vtkRectilinearGrid *
ReadRelevantGrid(const double *origin, const double *normal)
{
    int range[2];
    GetRelevantZRange(origin, normal, range);
    cerr << "Decided on " << range[0] << "-" << range[1] << endl;

    return ReadGrid(0, 511);
}


int main(int argc, char *argv[])
{   GetMemorySize("initialization");
    int t1;
    t1 = timer->StartTimer();


    double origin[3] = { 0.5, 0.5, 0.5 };
    double normal[3] = { 0.1, 0.2, 0.974679 };

    // Read the data.
    GetMemorySize("before read");
    vtkRectilinearGrid *rg = ReadRelevantGrid(origin, normal);
    GetMemorySize("after read");
    vtkCutter *cutter = vtkCutter::New();
    vtkPlane *plane = vtkPlane::New();
    plane->SetNormal(normal);
    plane->SetOrigin(origin);
    cutter->SetCutFunction(plane);
    cutter->SetInputData(rg);

    vtkDataSetMapper *mapper = vtkDataSetMapper::New();
    mapper->SetInputConnection(cutter->GetOutputPort());

    vtkLookupTable *lut = vtkLookupTable::New();
    mapper->SetLookupTable(lut);
    mapper->SetScalarRange(0,15);

    lut->Build();

    vtkActor *actor = vtkActor::New();
    actor->SetMapper(mapper);
//actor->GetProperty()->SetRepresentationToWireframe();
    vtkRenderer *ren = vtkRenderer::New();
    ren->AddActor(actor);

    vtkCamera *cam = ren->GetActiveCamera();
    cam->SetFocalPoint(0.5, 0.5, 0.5);
    cam->SetPosition(0.5, .5, 1.5);


    vtkRenderWindow *renwin = vtkRenderWindow::New();
    renwin->SetSize(768, 768);
    renwin->AddRenderer(ren);


    vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
    unsigned int n = 30;
    double opacity = 1.0 / (static_cast<double>(n)) * 5.0;

    plane->SetNormal(ren->GetActiveCamera()->GetViewPlaneNormal());
    plane->SetOrigin(ren->GetActiveCamera()->GetFocalPoint());


    iren->SetRenderWindow(renwin);
    GetMemorySize("end");
    timer->StopTimer(t1,"time");
    cutter->GenerateValues(n, -0.5, 0.5);
    lut -> SetAlphaRange(opacity, opacity);

    renwin->Render();
    iren->Start();
}


// You should not need to modify these routines.
vtkRectilinearGrid *
ReadGrid(int zStart, int zEnd)
{
    int  i;

    if (zStart < 0 || zEnd < 0 || zStart >= gridSize || zEnd >= gridSize || zStart > zEnd)
    {
        cerr << FICHIER << "  Invalid range: " << zStart << "-" << zEnd << endl;
        return NULL;
    }

    ifstream ifile(FICHIER);
    if (ifile.fail())
    {
        cerr << FICHIER << "  Unable to open file: " << FICHIER << "!!" << endl;
    }

    cerr << FICHIER << "  Reading from " << zStart << " to " << zEnd << endl;

    vtkRectilinearGrid *rg = vtkRectilinearGrid::New();
    vtkFloatArray *X = vtkFloatArray::New();
    X->SetNumberOfTuples(gridSize);
    for (i = 0 ; i < gridSize ; i++)
        X->SetTuple1(i, i/(gridSize-1.0));
    rg->SetXCoordinates(X);
    X->Delete();
    vtkFloatArray *Y = vtkFloatArray::New();
    Y->SetNumberOfTuples(gridSize);
    for (i = 0 ; i < gridSize ; i++)
        Y->SetTuple1(i, i/(gridSize-1.0));
    rg->SetYCoordinates(Y);
    Y->Delete();
    vtkFloatArray *Z = vtkFloatArray::New();
    int numSlicesToRead = zEnd-zStart+1;
    Z->SetNumberOfTuples(numSlicesToRead);
    for (i = zStart ; i <= zEnd ; i++)
        Z->SetTuple1(i-zStart, i/(gridSize-1.0));
    rg->SetZCoordinates(Z);
    Z->Delete();

    rg->SetDimensions(gridSize, gridSize, numSlicesToRead);

    int valuesPerSlice  = gridSize*gridSize;
    int bytesPerSlice   = 4*valuesPerSlice;

#if TAILLE == 512
    unsigned int offset          = (unsigned int)zStart * (unsigned int)bytesPerSlice;
    unsigned int bytesToRead     = bytesPerSlice*numSlicesToRead;
    unsigned int valuesToRead    = valuesPerSlice*numSlicesToRead;
#elif TAILLE == 1024
    unsigned long long offset          = (unsigned long long)zStart * bytesPerSlice;
    unsigned long long  bytesToRead     = (unsigned long long )bytesPerSlice*numSlicesToRead;
    unsigned int valuesToRead    = (unsigned int )valuesPerSlice*numSlicesToRead;
#else
#error Unsupported choice setting
#endif

    vtkFloatArray *scalars = vtkFloatArray::New();
    scalars->SetNumberOfTuples(valuesToRead);
    float *arr = scalars->GetPointer(0);
    ifile.seekg(offset, ios::beg);
    ifile.read((char *)arr, bytesToRead);
    ifile.close();

    scalars->SetName("entropy");
    rg->GetPointData()->AddArray(scalars);
    scalars->Delete();

/*
    vtkFloatArray *pr = vtkFloatArray::New();
    pr->SetNumberOfTuples(valuesToRead);
    for (int i = 0 ; i < valuesToRead ; i++)
        pr->SetTuple1(i, parRank);

    pr->SetName("par_rank");
    rg->GetPointData()->AddArray(pr);
    pr->Delete();
 */

    rg->GetPointData()->SetActiveScalars("entropy");

    cerr << FICHIER << "  Done reading" << endl;
    return rg;
}

void
WriteImage(const char *name, const float *rgba, int width, int height)
{
    vtkImageData *img = vtkImageData::New();
    img->SetDimensions(width, height, 1);
#if VTK_MAJOR_VERSION <= 5
    img->SetNumberOfScalarComponents(3);
    img->SetScalarTypeToUnsignedChar();
#else
    img->AllocateScalars(VTK_UNSIGNED_CHAR,3);
#endif

    for (int i = 0 ; i < width ; i++)
        for (int j = 0 ; j < height ; j++)
        {
            unsigned char *ptr = (unsigned char *) img->GetScalarPointer(i, j, 0);
            int idx = j*width + i;
            ptr[0] = (unsigned char) (255*rgba[4*idx + 0]);
            ptr[1] = (unsigned char) (255*rgba[4*idx + 1]);
            ptr[2] = (unsigned char) (255*rgba[4*idx + 2]);
        }


    vtkPNGWriter *writer = vtkPNGWriter::New();
    writer->SetInputData(img);
    writer->SetFileName(name);
    writer->Write();

    img->Delete();
    writer->Delete();

}


