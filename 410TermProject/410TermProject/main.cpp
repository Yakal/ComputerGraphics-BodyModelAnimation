//
//  Comp410 Term Project
//  Sitare Arslantürk 57677
//  Furkan Yakal 59943
//
#define GL_SILENCE_DEPRECATION
#include "Angel.h"

typedef vec4  color4;
typedef vec4  point4;
///
//////--------------------------------------------------- SPHERE ENTITIES  ---------------------------------------------------------//////
const int NumTimesToSubdivide = 5;
const int NumTriangles = 4096;
// (4 faces)^(NumTimesToSubdivide + 1)
const int NumVerticesSphere = 3 * NumTriangles;

point4 pointSphere[15][NumVerticesSphere];
vec3 normals[15][NumVerticesSphere];

int IndexSphere = 0;
void
triangle( const point4& a, const point4& b, const point4& c , int i )
{
    vec3 normal = normalize( cross(b - a, c - b) );
    normals[i][IndexSphere] = normal; pointSphere[i][IndexSphere] = a; IndexSphere++;
    normals[i][IndexSphere] = normal; pointSphere[i][IndexSphere] = b; IndexSphere++;
    normals[i][IndexSphere] = normal; pointSphere[i][IndexSphere] = c; IndexSphere++;
    if(IndexSphere==NumVerticesSphere)IndexSphere=0;
}
point4
unit( const point4& p )
{
    float len = p.x*p.x + p.y*p.y + p.z*p.z;
    point4 t;
    if ( len > DivideByZeroTolerance ) {
        t = p / sqrt(len);
        t.w = 1.0;
    }
    return t;
}
void
divide_triangle( const point4& a, const point4& b,
                const point4& c, int count, int i )
{
    if ( count > 0 ) {
        point4 v1 = unit( a + b );
        point4 v2 = unit( a + c );
        point4 v3 = unit( b + c );
        divide_triangle( a, v1, v2, count - 1 , i);
        divide_triangle( c, v2, v3, count - 1 , i);
        divide_triangle( b, v3, v1, count - 1 , i);
        divide_triangle( v1, v3, v2, count - 1 , i);
    }
    else {
        triangle( a, b, c , i);
    }
}
void
tetrahedron( int count)
{
    for(int i=0 ; i<15; i++){
        point4 v[4] = {
            vec4( 0.0, 0.0, 1.0, 1.0 ),
            vec4( 0.0, 0.942809, -0.333333, 1.0 ),
            vec4( -0.816497, -0.471405, -0.333333, 1.0 ),
            vec4( 0.816497, -0.471405, -0.333333, 1.0 )
        };
        divide_triangle( v[0], v[1], v[2], count, i);
        divide_triangle( v[3], v[2], v[1], count, i );
        divide_triangle( v[0], v[3], v[1], count, i );
        divide_triangle( v[0], v[2], v[3], count, i );
    }
}
void createSpheres(){
    tetrahedron(NumTimesToSubdivide);
}

float maleBodyHeight = 50;
float femaleBodyHeight = 50;

//male body ratios
float male_center_chest = 1.9/7.7;
float male_chest_head = 1.4/7.7;

float male_center_hipX = 0.3/7.7;
float male_center_hipY = 0.6/7.7;

float male_hip_kneeX = 0.1/7.7;
float male_hip_kneeY = 2/7.7;

float male_knee_ankleX = 0.1/7.7;
float male_knee_ankleY = 2.2/7.7;

float male_chest_shoulderX = 1/7.7;
float male_chest_shoulderY = 0.3/7.7;

float male_shoulder_elbowX = 0.1/7.7;
float male_shoulder_elbowY = 1.4/7.7;

float male_elbow_wristX = 0.1/7.7;
float male_elbow_wristY = 1.3/7.7;

//female body ratios
float female_center_chest = 1.8/7.4;
float female_chest_head = 1.4/7.4;

float female_center_hipX = 0.6/7.4;
float female_center_hipY = 0.6/7.4;

float female_hip_kneeX = 0.3/7.4;
float female_hip_kneeY = 2.1/7.4;

float female_knee_ankleX = 0.1/7.4;
float female_knee_ankleY = 2.2/7.4;

float female_chest_shoulderX = 0.8/7.4;
float female_chest_shoulderY = 0.2/7.4;

float female_shoulder_elbowX = 0.15/7.4;
float female_shoulder_elbowY = 1.4/7.4;

float female_elbow_wristX = 0.2/7.4;
float female_elbow_wristY = 1.3/7.4;

enum {Center =0, Chest=1, Head=2, RightHip=3 , RightKnee=4, RightAnkle=5, LeftHip=6 , LeftKnee=7, LeftAnkle=8,
    RightShoulder=9 , RightElbow=10, RightWrist=11,  LeftShoulder=12 , LeftElbow=13, LeftWrist=14};

enum{Male=0, Female=1};
enum{Walking=0, Running=1, Calm=2, Angry=3};

int movType=Walking;
int gender=Male;

bool walking=false;
bool running=false;
bool isCalm = false;
bool isAngry=false;

bool isStarted=false;

point4 coordinates[2][15]; //0 for male 1 for female

point4 center;
point4 chest;
point4 head;

point4 rightHip;
point4 rightKnee;
point4 rightAnkle;

point4 leftHip;
point4 leftKnee;
point4 leftAnkle;

point4 rightShoulder;
point4 rightElbow;
point4 rightWrist;

point4 leftShoulder;
point4 leftElbow;
point4 leftWrist;

void setUpMaleBody(float bodyHeight){
    center = {0,0,0,1};
    chest = {center.x, bodyHeight*male_center_chest, center.z,1};
    head = {chest.x, bodyHeight*male_chest_head, chest.z,1};
    
    rightHip = {bodyHeight*male_center_hipX, -bodyHeight*male_center_hipY, center.z,1};
    rightKnee = {bodyHeight*male_hip_kneeX, -bodyHeight*male_hip_kneeY, rightHip.z,1};
    rightAnkle = {bodyHeight*male_knee_ankleX, -bodyHeight*male_knee_ankleY, rightKnee.z,1};
    
    leftHip = {-bodyHeight*male_center_hipX, -bodyHeight*male_center_hipY, center.z,1};
    leftKnee = {-bodyHeight*male_hip_kneeX, -bodyHeight*male_hip_kneeY, leftHip.z,1};
    leftAnkle = {-bodyHeight*male_knee_ankleX, -bodyHeight*male_knee_ankleY, leftKnee.z,1};
    
    rightShoulder = {bodyHeight*male_chest_shoulderX, bodyHeight*male_chest_shoulderY, chest.z,1};
    rightElbow = {bodyHeight*male_shoulder_elbowX, -bodyHeight*male_shoulder_elbowY, rightShoulder.z,1};
    rightWrist = {bodyHeight*male_elbow_wristX, -bodyHeight*male_elbow_wristY, rightElbow.z,1};
    
    leftShoulder = {-bodyHeight*male_chest_shoulderX, bodyHeight*male_chest_shoulderY, chest.z,1};
    leftElbow = {-bodyHeight*male_shoulder_elbowX, -bodyHeight*male_shoulder_elbowY, leftShoulder.z,1};
    leftWrist = {-bodyHeight*male_elbow_wristX, -bodyHeight*male_elbow_wristY, leftElbow.z,1};
    
    coordinates[0][0]=center;
    coordinates[0][1]=chest;
    coordinates[0][2]=head;
    coordinates[0][3]=rightHip;
    coordinates[0][4]=rightKnee;
    coordinates[0][5]=rightAnkle;
    coordinates[0][6]=leftHip;
    coordinates[0][7]=leftKnee;
    coordinates[0][8]=leftAnkle;
    coordinates[0][9]=rightShoulder;
    coordinates[0][10]=rightElbow;
    coordinates[0][11]=rightWrist;
    coordinates[0][12]=leftShoulder;
    coordinates[0][13]=leftElbow;
    coordinates[0][14]=leftWrist;
}
void setUpFemaleBody(float bodyHeight){
    center = {0,0,0,1};
    chest = {center.x, bodyHeight*female_center_chest, center.z,1};
    head = {chest.x, bodyHeight*female_chest_head, chest.z,1};
    
    rightHip = {bodyHeight*female_center_hipX, -bodyHeight*female_center_hipY, center.z,1};
    rightKnee = {-bodyHeight*female_hip_kneeX, -bodyHeight*female_hip_kneeY, rightHip.z,1};
    rightAnkle = {-bodyHeight*female_knee_ankleX, -bodyHeight*female_knee_ankleY, rightKnee.z,1};
    
    leftHip = {-bodyHeight*female_center_hipX, -bodyHeight*female_center_hipY, center.z,1};
    leftKnee = {bodyHeight*female_hip_kneeX, -bodyHeight*female_hip_kneeY, leftHip.z,1};
    leftAnkle = {bodyHeight*female_knee_ankleX, -bodyHeight*female_knee_ankleY, leftKnee.z,1};
    
    rightShoulder = {bodyHeight*female_chest_shoulderX, bodyHeight*female_chest_shoulderY, chest.z,1};
    rightElbow = {bodyHeight*female_shoulder_elbowX, -bodyHeight*female_shoulder_elbowY, rightShoulder.z,1};
    rightWrist = {bodyHeight*female_elbow_wristX, -bodyHeight*female_elbow_wristY, rightElbow.z,1};
    
    leftShoulder = {-bodyHeight*female_chest_shoulderX, bodyHeight*female_chest_shoulderY, chest.z,1};
    leftElbow = {-bodyHeight*female_shoulder_elbowX, -bodyHeight*female_shoulder_elbowY, leftShoulder.z,1};
    leftWrist = {-bodyHeight*female_elbow_wristX, -bodyHeight*female_elbow_wristY, leftElbow.z,1};
    
    coordinates[1][0]=center;
    coordinates[1][1]=chest;
    coordinates[1][2]=head;
    coordinates[1][3]=rightHip;
    coordinates[1][4]=rightKnee;
    coordinates[1][5]=rightAnkle;
    coordinates[1][6]=leftHip;
    coordinates[1][7]=leftKnee;
    coordinates[1][8]=leftAnkle;
    coordinates[1][9]=rightShoulder;
    coordinates[1][10]=rightElbow;
    coordinates[1][11]=rightWrist;
    coordinates[1][12]=leftShoulder;
    coordinates[1][13]=leftElbow;
    coordinates[1][14]=leftWrist;
}
void setUpInitialBodies (){
    setUpMaleBody(maleBodyHeight);
    setUpFemaleBody(femaleBodyHeight);
}
vec3 getDisplacement(int index){
    return vec3(coordinates[gender][index].x,coordinates[gender][index].y,coordinates[gender][index].z );
}
const GLfloat AnglesConst[4][15][3]={{//WALKING
    {0,-30,0},//CENTER
    {0,0,0},//CHEST
    {0,0,0},//HEAD
    
    {-35,0,0},//RIGHT HIP
    {30,0,0},//RIGHT KNEE
    {0,0,0},//RIGHT ANKLE
    
    {25,0,0},//LEFT HIP
    {5,0,0},//LEFT KNEE
    {0,0,0},//LEFT ANKLE
    
    {40,0,0},//RIGHT SHOULDER
    {-40,0,0},//RIGHT ELBOW
    {0,0,0},//RIGHT WRIST
    
    {-20,0,0},//LEFT SHOULDER
    {-40,0,0},//LEFT ELBOW
    {0,0,0},//LEFT WRIST
},
    {//RUNNING
        {0,-30,0},//CENTER
        {0,0,0},//CHEST
        {0,0,0},//HEAD
        
        {-45,0,0},//RIGHT HIP
        {30,0,0},//RIGHT KNEE
        {0,0,0},//RIGHT ANKLE
        
        {35,0,0},//LEFT HIP
        {90,0,0},//LEFT KNEE
        {0,0,0},//LEFT ANKLE
        
        {45,0,0},//RIGHT SHOULDER
        {-90,0,0},//RIGHT ELBOW
        {0,0,0},//RIGHT WRIST
        
        {-35,0,0},//LEFT SHOULDER
        {-90,0,0},//LEFT ELBOW
        {0,0,0},//LEFT WRIST
    },{//CALM
        {0,-30,0},//CENTER
        {0,0,0},//CHEST
        {0,0,0},//HEAD
        
        {-20,0,0},//RIGHT HIP
        {30,0,0},//RIGHT KNEE
        {0,0,0},//RIGHT ANKLE
        
        {10,0,0},//LEFT HIP
        {5,0,0},//LEFT KNEE
        {0,0,0},//LEFT ANKLE
        
        {15,0,-10},//RIGHT SHOULDER
        {0,0,0},//RIGHT ELBOW
        {0,0,0},//RIGHT WRIST
        
        {-15,0,10},//LEFT SHOULDER
        {0,0,0},//LEFT ELBOW
        {0,0,0},//LEFT WRIST
    },
    {//ANGRY
        {0,-30,0},//CENTER
        {0,0,0},//CHEST
        {0,0,0},//HEAD
        
        {-40,0,0},//RIGHT HIP
        {30,0,0},//RIGHT KNEE
        {0,0,0},//RIGHT ANKLE
        
        {30,0,0},//LEFT HIP
        {5,0,0},//LEFT KNEE
        {0,0,0},//LEFT ANKLE
        
        {45,0,10},//RIGHT SHOULDER
        {-5,5,0},//RIGHT ELBOW
        {0,0,0},//RIGHT WRIST
        
        {-25,0,-10},//LEFT SHOULDER
        {-40,-5,0},//LEFT ELBOW
        {0,0,0},//LEFT WRIST
    },
};

GLfloat Angles[2][15][3]={{//WALKING
    {0,-90,0},//CENTER
    {0,0,0},//CHEST
    {0,0,0},//HEAD
    
    {-35,0,0},//RIGHT HIP
    {30,0,0},//RIGHT KNEE
    {0,0,0},//RIGHT ANKLE
    
    {25,0,0},//LEFT HIP
    {5,0,0},//LEFT KNEE
    {0,0,0},//LEFT ANKLE
    
    {40,0,0},//RIGHT SHOULDER
    {-40,0,0},//RIGHT ELBOW
    {0,0,0},//RIGHT WRIST
    
    {-20,0,0},//LEFT SHOULDER
    {-40,0,0},//LEFT ELBOW
    {0,0,0},//LEFT WRIST
},
    {//RUNNING
        {0,-90,0},//CENTER
        {0,0,0},//CHEST
        {0,0,0},//HEAD
        
        {-45,0,0},//RIGHT HIP
        {30,0,0},//RIGHT KNEE
        {0,0,0},//RIGHT ANKLE
        
        {35,0,0},//LEFT HIP
        {90,0,0},//LEFT KNEE
        {0,0,0},//LEFT ANKLE
        
        {45,0,0},//RIGHT SHOULDER
        {-90,0,0},//RIGHT ELBOW
        {0,0,0},//RIGHT WRIST
        
        {-35,0,0},//LEFT SHOULDER
        {-90,0,0},//LEFT ELBOW
        {0,0,0},//LEFT WRIST
    }
};

mat4 getRotationMatrix(int index){
    return RotateX(Angles[movType][index][0])* RotateY(Angles[movType][index][1])*RotateZ(Angles[movType][index][2]);
}

//--------------------------------------------------------------------------------------------------------//
// Array of rotation angles (in degrees) for each coordinate axis
//this is needed for rotating the bunny's initial prespective
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int  Axis = Xaxis;
//needed rotating for bunny's initial perspective. -90 degrees in x axis, and 90 degrees in z axis
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };
// ModelView, Projection, and ColorUpdate matrices uniform location
GLuint  ModelView, Projection, ColorUpdater;
GLuint vao[29];
GLuint vPosition;
GLuint vColor;
mat4 model_view[15];

void initializeModelViews(int gender){
    model_view[Center] = Scale(0.02, 0.02, 0.02)*Translate(getDisplacement(Center))*getRotationMatrix(Center);
    model_view[Chest] = model_view[Center]*Translate(getDisplacement(Chest))*getRotationMatrix(Chest);
    model_view[Head] = model_view[Chest]*Translate(getDisplacement(Head))*getRotationMatrix(Head);
    
    model_view[RightHip] = model_view[Center]*getRotationMatrix(RightHip)*Translate(getDisplacement(RightHip));
    model_view[RightKnee] =model_view[RightHip]*Translate(getDisplacement(RightKnee))*getRotationMatrix(RightKnee);
    model_view[RightAnkle] = model_view[RightKnee]*Translate(getDisplacement(RightAnkle))*getRotationMatrix(RightAnkle);
    
    model_view[LeftHip] = model_view[Center]*getRotationMatrix(LeftHip)*Translate(getDisplacement(LeftHip));
    model_view[LeftKnee] = model_view[LeftHip]*Translate(getDisplacement(LeftKnee))*getRotationMatrix(LeftKnee);
    model_view[LeftAnkle] = model_view[LeftKnee]*Translate(getDisplacement(LeftAnkle))*getRotationMatrix(LeftAnkle);
    
    model_view[RightShoulder] = model_view[Chest]*getRotationMatrix(RightShoulder)*Translate(getDisplacement(RightShoulder));
    model_view[RightElbow] = model_view[RightShoulder]*Translate(getDisplacement(RightElbow))*getRotationMatrix(RightElbow);
    model_view[RightWrist] = model_view[RightElbow]*Translate(getDisplacement(RightWrist))*getRotationMatrix(RightWrist);
    
    model_view[LeftShoulder] = model_view[Chest]*getRotationMatrix(LeftShoulder)*Translate(getDisplacement(LeftShoulder));
    model_view[LeftElbow] = model_view[LeftShoulder]*Translate(getDisplacement(LeftElbow))*getRotationMatrix(LeftElbow);
    model_view[LeftWrist] = model_view[LeftElbow]*Translate(getDisplacement(LeftWrist))*getRotationMatrix(LeftWrist);
}

//----------------------------------------------------------------------------
// OpenGL initialization
//Used multiple vertex array objects to change between the object types
void
init()
{
    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    // Create a vertex array object
    glGenVertexArrays( 15, vao );
    //------------------------------------------SPHERE RELATED-------------------------------------------------//
    createSpheres();
    
    for(int i=0 ; i<15 ; i++){
        glBindVertexArray( vao[i] );
        // Create and initialize a buffer object
        GLuint buffer;
        glGenBuffers( 1, &buffer );
        glBindBuffer( GL_ARRAY_BUFFER, buffer );
        glBufferData( GL_ARRAY_BUFFER, sizeof(pointSphere[i]) + sizeof(normals[i]),NULL, GL_STATIC_DRAW );
        glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(pointSphere[i]), pointSphere[i] );
        glBufferSubData( GL_ARRAY_BUFFER, sizeof(pointSphere[i]), sizeof(normals[i]), normals[i]);
        // set up vertex arrays
        glEnableVertexAttribArray( vPosition );
        glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
        model_view[i] = identity();
        
    }
    setUpInitialBodies();
    initializeModelViews(Male);
    
    
    
    ModelView = glGetUniformLocation( program, "ModelView" );
    Projection = glGetUniformLocation( program, "Projection" );
    
    // Set current program object
    glUseProgram( program );
    // Enable hiddden surface removal
    glEnable( GL_DEPTH_TEST );
    // Set state variable "clear color" to clear buffer with.
    glClearColor(1.0, 1.0, 1.0, 1.0 ); // white background
}

bool isRotateLeft =false;
bool isRotateRight = false;

bool rotateRightArmForward=true;
bool rotateRightArmBackward=false;

bool rotateLeftArmForward=false;
bool rotateLeftArmBackward=true;

bool rotateRightLegForward=false;
bool rotateRightLegBackward=true;

bool rotateLeftLegForward=true;
bool rotateLeftLegBackward=false;

void updateModelViews(){
    model_view[Chest] = model_view[Center]*Translate(getDisplacement(Chest))*getRotationMatrix(Chest);
    model_view[Head] = model_view[Chest]*Translate(getDisplacement(Head))*getRotationMatrix(Head);
    
    model_view[RightHip] = model_view[Center]*getRotationMatrix(RightHip)*Translate(getDisplacement(RightHip));
    model_view[RightKnee] =model_view[RightHip]*Translate(getDisplacement(RightKnee))*getRotationMatrix(RightKnee);
    model_view[RightAnkle] = model_view[RightKnee]*Translate(getDisplacement(RightAnkle))*getRotationMatrix(RightAnkle);
    
    model_view[LeftHip] = model_view[Center]*getRotationMatrix(LeftHip)*Translate(getDisplacement(LeftHip));
    model_view[LeftKnee] = model_view[LeftHip]*Translate(getDisplacement(LeftKnee))*getRotationMatrix(LeftKnee);
    model_view[LeftAnkle] = model_view[LeftKnee]*Translate(getDisplacement(LeftAnkle))*getRotationMatrix(LeftAnkle);
    
    model_view[RightShoulder] = model_view[Chest]*getRotationMatrix(RightShoulder)*Translate(getDisplacement(RightShoulder));
    model_view[RightElbow] = model_view[RightShoulder]*Translate(getDisplacement(RightElbow))*getRotationMatrix(RightElbow);
    model_view[RightWrist] = model_view[RightElbow]*Translate(getDisplacement(RightWrist))*getRotationMatrix(RightWrist);
    
    model_view[LeftShoulder] = model_view[Chest]*getRotationMatrix(LeftShoulder)*Translate(getDisplacement(LeftShoulder));
    model_view[LeftElbow] = model_view[LeftShoulder]*Translate(getDisplacement(LeftElbow))*getRotationMatrix(LeftElbow);
    model_view[LeftWrist] = model_view[LeftElbow]*Translate(getDisplacement(LeftWrist))*getRotationMatrix(LeftWrist);
}
//-------------------------------------
void updateRightArm(){
    model_view[RightShoulder] = model_view[Chest]*getRotationMatrix(RightShoulder)*Translate(getDisplacement(RightShoulder));
    model_view[RightElbow] = model_view[RightShoulder]*Translate(getDisplacement(RightElbow))*getRotationMatrix(RightElbow);
    model_view[RightWrist] = model_view[RightElbow]*Translate(getDisplacement(RightWrist))*getRotationMatrix(RightWrist);
}
void updateLeftArm(){
    
    model_view[LeftShoulder] = model_view[Chest]*getRotationMatrix(LeftShoulder)*Translate(getDisplacement(LeftShoulder));
    model_view[LeftElbow] = model_view[LeftShoulder]*Translate(getDisplacement(LeftElbow))*getRotationMatrix(LeftElbow);
    model_view[LeftWrist] = model_view[LeftElbow]*Translate(getDisplacement(LeftWrist))*getRotationMatrix(LeftWrist);
}
void updateRightLeg(){
    model_view[RightHip] = model_view[Center]*getRotationMatrix(RightHip)*Translate(getDisplacement(RightHip));
    model_view[RightKnee] =model_view[RightHip]*Translate(getDisplacement(RightKnee))*getRotationMatrix(RightKnee);
    model_view[RightAnkle] = model_view[RightKnee]*Translate(getDisplacement(RightAnkle))*getRotationMatrix(RightAnkle);
}
void updateLeftLeg(){
    model_view[LeftHip] = model_view[Center]*getRotationMatrix(LeftHip)*Translate(getDisplacement(LeftHip));
    model_view[LeftKnee] = model_view[LeftHip]*Translate(getDisplacement(LeftKnee))*getRotationMatrix(LeftKnee);
    model_view[LeftAnkle] = model_view[LeftKnee]*Translate(getDisplacement(LeftAnkle))*getRotationMatrix(LeftAnkle);
}
//------------------------------------
void rotateRightArm(){
    if(rotateRightArmForward){
        model_view[RightShoulder]=model_view[RightShoulder]*Translate(getDisplacement(RightShoulder))*RotateX(-1)*Translate(-getDisplacement(RightShoulder));
        Angles[movType][RightShoulder][Xaxis]-=1;
        
        //model_view[RightElbow]=model_view[RightElbow]*Translate(getDisplacement(RightElbow))*RotateX((float)15/30)*Translate(-getDisplacement(RightElbow));
        //Angles[RightElbow][Xaxis]+=(float)15/30;
    }else if(rotateRightArmBackward){
        model_view[RightShoulder]=model_view[RightShoulder]*Translate(getDisplacement(RightShoulder))*RotateX(1)*Translate(-getDisplacement(RightShoulder));
        Angles[movType][RightShoulder][Xaxis]+=1;
        //model_view[RightElbow]=model_view[RightElbow]*Translate(getDisplacement(RightElbow))*RotateX((float)-15/30)*Translate(-getDisplacement(RightElbow));
        //Angles[RightElbow][Xaxis]-=(float)15/30;
        
    }
    updateRightArm();
}
void rotateLeftArm(){
    if(rotateLeftArmForward){
        model_view[LeftShoulder]=model_view[LeftShoulder]*Translate(getDisplacement(LeftShoulder))*RotateX(-1)*Translate(-getDisplacement(LeftShoulder));
        Angles[movType][LeftShoulder][Xaxis]-=1;
        //model_view[LeftElbow]=model_view[LeftElbow]*Translate(getDisplacement(LeftElbow))*RotateX((float)15/30)*Translate(-getDisplacement(LeftElbow));
        //Angles[LeftElbow][Xaxis]+=(float)15/30;
    }
    else if(rotateLeftArmBackward){
        model_view[LeftShoulder]=model_view[LeftShoulder]*Translate(getDisplacement(LeftShoulder))*RotateX(+1)*Translate(-getDisplacement(LeftShoulder));
        Angles[movType][LeftShoulder][Xaxis]+=1;
        //model_view[LeftElbow]=model_view[LeftElbow]*Translate(getDisplacement(LeftElbow))*RotateX((float)-15/30)*Translate(-getDisplacement(LeftElbow));
        //Angles[LeftElbow][Xaxis]-=(float)15/30;
    }
    updateLeftArm();
    
}
float legRotForMovType[4];


void rotateRightLeg(){
    if(rotateRightLegForward){
        legRotForMovType[0]=(float)25/60;
        legRotForMovType[1]=(float)-60/80;
        
        model_view[RightHip]=model_view[RightHip]*Translate(getDisplacement(RightHip))*RotateX(-1)*Translate(-getDisplacement(RightHip));
        Angles[movType][RightHip][Xaxis]-=1;
        updateRightLeg();
        
        model_view[RightKnee]=model_view[RightKnee]*Translate(getDisplacement(RightKnee))*RotateX(legRotForMovType[movType])*Translate(-getDisplacement(RightKnee));
        Angles[movType][RightKnee][Xaxis]+=legRotForMovType[movType];
        updateRightLeg();
    }else if(rotateRightLegBackward){
        legRotForMovType[0]=(float)-25/60;
        legRotForMovType[1]=(float)60/80;
        
        model_view[RightHip]=model_view[RightHip]*Translate(getDisplacement(RightHip))*RotateX(+1)*Translate(-getDisplacement(RightHip));
        Angles[movType][RightHip][Xaxis]+=1;
        updateRightLeg();
        
        model_view[RightKnee]=model_view[RightKnee]*Translate(getDisplacement(RightKnee))*RotateX(legRotForMovType[movType])*Translate(-getDisplacement(RightKnee));
        Angles[movType][RightKnee][Xaxis]+=legRotForMovType[movType];
        updateRightLeg();
    }
}

void rotateLeftLeg(){
    if(rotateLeftLegForward){
        legRotForMovType[0]=(float)25/60;
        legRotForMovType[1]=(float)-60/80;
        legRotForMovType[2]=(float)25/30;
        legRotForMovType[3]=(float)25/70;
        
        model_view[LeftHip]=model_view[LeftHip]*Translate(getDisplacement(LeftHip))*RotateX(-1)*Translate(-getDisplacement(LeftHip));
        Angles[movType][LeftHip][Xaxis]-=1;
        updateLeftLeg();
        
        model_view[LeftKnee]=model_view[LeftKnee]*Translate(getDisplacement(LeftKnee))*RotateX(legRotForMovType[movType])*Translate(-getDisplacement(LeftKnee));
        Angles[movType][LeftKnee][Xaxis]+=legRotForMovType[movType];
        updateLeftLeg();
        
    }else if(rotateLeftLegBackward){
        legRotForMovType[0]=(float)-25/60;
        legRotForMovType[1]=(float)60/80;
        legRotForMovType[2]=(float)-25/30;
        legRotForMovType[3]=(float)-25/70;
        
        model_view[LeftHip]=model_view[LeftHip]*Translate(getDisplacement(LeftHip))*RotateX(1)*Translate(-getDisplacement(LeftHip));
        Angles[movType][LeftHip][Xaxis]+=1;
        updateLeftLeg();
        
        model_view[LeftKnee]=model_view[LeftKnee]*Translate(getDisplacement(LeftKnee))*RotateX(legRotForMovType[movType])*Translate(-getDisplacement(LeftKnee));
        Angles[movType][LeftKnee][Xaxis]+=legRotForMovType[movType];
        updateLeftLeg();
    }
}
int chooseOption=0;
void hipInZAxis(){
    float angle = 0.05;
    if(isAngry)angle=0.1;
    if(chooseOption==0){
        model_view[LeftHip] = RotateZ(-angle)*model_view[LeftHip];
        Angles[movType][LeftHip][Zaxis]-=angle;
        
        model_view[RightHip] = RotateZ(-angle)*model_view[RightHip];
        Angles[movType][RightHip][Zaxis]-=angle;
        
    }else if(chooseOption==1){
        model_view[RightHip] = RotateZ(+angle)*model_view[RightHip];
        Angles[movType][RightHip][Zaxis]+=angle;
        
        model_view[LeftHip] = RotateZ(+angle)*model_view[LeftHip];
        Angles[movType][LeftHip][Zaxis]+=angle;
        
    }else if(chooseOption==2){
        model_view[LeftHip] = RotateZ(+angle)*model_view[LeftHip];
        Angles[movType][LeftHip][Zaxis]+=angle;
        
        model_view[RightHip] = RotateZ(+angle)*model_view[RightHip];
        Angles[movType][RightHip][Zaxis]+=angle;
        
    }else if(chooseOption==3){
        model_view[LeftHip] = RotateZ(-angle)*model_view[LeftHip];
        Angles[movType][LeftHip][Zaxis]-=angle;
        
        model_view[RightHip] = RotateZ(-angle)*model_view[RightHip];
        Angles[movType][RightHip][Zaxis]-=angle;
        
    }
    
    chooseOption++;
    chooseOption=chooseOption%4;
    updateLeftLeg();
    updateRightLeg();
}

void bodyOssilation(){
    if(rotateLeftLegForward){
        model_view[Center] = RotateZ(-0.05)*model_view[Center];
        
    }else if(rotateRightLegForward){
        model_view[Center] = RotateZ(+0.05)*model_view[Center];
    }
    updateModelViews();
}

void rotateBody(float angle){
    model_view[Center]=RotateY(angle)*model_view[Center];
    updateModelViews();
}



void
display( void )
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(isRotateRight)
        rotateBody(1);
    else if(isRotateLeft)
        rotateBody(-1);
    if(isStarted){
        rotateRightLeg();
        rotateLeftLeg();
        rotateRightArm();
        rotateLeftArm();
        hipInZAxis();
        bodyOssilation();
    }
    updateModelViews();
    for(int i=0; i<15 ;i++){
        glBindVertexArray(vao[i]);
        glUniformMatrix4fv( ModelView, 1, GL_TRUE, model_view[i] );
        glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    }
    glutSwapBuffers();
}

//--------------------------------------------------------------------------------------------------------//
//Rearrenges the size and position of the object when the window size is changed.
void reshape( int w, int h )
{
    glViewport( 0, 0, w, h );
    // Set projection matrix
    mat4  projection;
    if (w <= h)
        projection = Ortho(-1.0, 1.0, -1.0 * (GLfloat) h / (GLfloat) w,
                           1.0 * (GLfloat) h / (GLfloat) w, -1.0, 1.0);
    else  projection = Ortho(-1.0* (GLfloat) w / (GLfloat) h, 1.0 *
                             (GLfloat) w / (GLfloat) h, -1.0, 1.0, -1.0, 1.0);
    glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );
}
//--------------------------------------------------------------------------------------------------------//
bool isStopped=false;
void
keyboard( unsigned char key, int x, int y )
{
    if(key == 'Q' | key == 'q')
        exit(0);
    if(key== 'S' | key== 's'){
        isStarted=true;
        isStopped=false;
        if(movType==Walking)walking=true;
        else if(movType==Running)running=true;
        else if(movType==Calm)isCalm=true;
        else if(movType==Angry)isAngry=true;
        
    }
    if(key== 'D' | key== 'd'){
        isStarted=false;
        walking=false;
        running=false;
        isCalm=false;
        isAngry=false;
        isStopped=true;
        
    }
    
    if(key == 'h' | key == 'H'){
        std::cout<< "Body Stick Model Animation "<<std::endl;
    }
}
//--------------------------------------------------------------------------------------------------------//
void mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
        isRotateLeft=true;
    else if(state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON)
        isRotateRight=true;
    
    if(state==GLUT_UP){
        isRotateRight=false;
        isRotateLeft=false;
    }
}
//--------------------------------------------------------------------------------------------------------//
int legCounterW=0;
int armCounterW=0;

int legCounterR=0;
int armCounterR=0;

int legCounterC=0;
int armCounterC=0;

int legCounterA=0;
int armCounterA=0;

void timer( int p )
{
    if(Theta[Yaxis]>=360) Theta[Yaxis]-=360;
    if(Theta[Yaxis]<=-360) Theta[Yaxis]+=360;
    if(!isStopped){
        if(walking){
            legCounterW++;
            if(legCounterW==30){
                //std::cout<< rotateLeftLegForward <<std::endl;
                rotateRightLegForward=!rotateRightLegForward;
                rotateRightLegBackward=!rotateRightLegBackward;
                
                rotateLeftLegForward=!rotateLeftLegForward;
                rotateLeftLegBackward=!rotateLeftLegBackward;
                legCounterW=0;
            }
            armCounterW++;
            if(armCounterW==30){
                rotateRightArmForward=!rotateRightArmForward;
                rotateRightArmBackward=!rotateRightArmBackward;
                
                rotateLeftArmForward=!rotateLeftArmForward;
                rotateLeftArmBackward=!rotateLeftArmBackward;
                armCounterW=0;
            }
        }else if(running){
            legCounterR++;
            if(legCounterR==40){
                rotateRightLegForward=!rotateRightLegForward;
                rotateRightLegBackward=!rotateRightLegBackward;
                
                rotateLeftLegForward=!rotateLeftLegForward;
                rotateLeftLegBackward=!rotateLeftLegBackward;
                legCounterR=0;
                
            }
            armCounterR++;
            if(armCounterR==40){
                rotateRightArmForward=!rotateRightArmForward;
                rotateRightArmBackward=!rotateRightArmBackward;
                
                rotateLeftArmForward=!rotateLeftArmForward;
                rotateLeftArmBackward=!rotateLeftArmBackward;
                armCounterR=0;
            }
        }else if(isCalm){
            legCounterC++;
            if(legCounterC==15){
                rotateRightLegForward=!rotateRightLegForward;
                rotateRightLegBackward=!rotateRightLegBackward;
                
                rotateLeftLegForward=!rotateLeftLegForward;
                rotateLeftLegBackward=!rotateLeftLegBackward;
                
                legCounterC=0;
            }
            armCounterC++;
            if(armCounterC==15){
                rotateRightArmForward=!rotateRightArmForward;
                rotateRightArmBackward=!rotateRightArmBackward;
                
                rotateLeftArmForward=!rotateLeftArmForward;
                rotateLeftArmBackward=!rotateLeftArmBackward;
                armCounterC=0;
            }
        }else if(isAngry){
            legCounterA++;
            if(legCounterA==35){
                rotateRightLegForward=!rotateRightLegForward;
                rotateRightLegBackward=!rotateRightLegBackward;
                
                rotateLeftLegForward=!rotateLeftLegForward;
                rotateLeftLegBackward=!rotateLeftLegBackward;
                
                legCounterA=0;
            }
            armCounterA++;
            if(armCounterA==35){
                rotateRightArmForward=!rotateRightArmForward;
                rotateRightArmBackward=!rotateRightArmBackward;
                
                rotateLeftArmForward=!rotateLeftArmForward;
                rotateLeftArmBackward=!rotateLeftArmBackward;
                armCounterA=0;
            }
        }
    }
    
    glutPostRedisplay();
    if(isCalm){
        glutTimerFunc(30,timer,0);
    }else if(isAngry){
        glutTimerFunc(10,timer,0);
    }else{
        glutTimerFunc(15,timer,0);
    }
    
}

void genderType(int n){
    if(gender!=n){
        gender=n;
        updateModelViews();
    }
}

void age(int n){
    if(n==0){
        maleBodyHeight=30;
        femaleBodyHeight=30;
    }else if(n==1){
        maleBodyHeight=50;
        femaleBodyHeight=50;
    }else if(n==2){
        maleBodyHeight=40;
        femaleBodyHeight=40;
    }
    setUpInitialBodies();
    updateModelViews();
}

void speed(int n){
    if(n==0){
        walking=true;
        running=false;
        isCalm=false;
        isAngry=false;
        movType=Walking;
    }else if(n==1){
        walking=false;
        running=true;
        isCalm=false;
        isAngry=false;
        movType=Running;
    }
    for(int i=0 ; i<4 ; i++){
        for (int j=0; j<15; j++){
            for(int k=0; k<3; k++){
                Angles[i][j][k] = AnglesConst[i][j][k];
            }
        }
    }
    rotateRightArmForward=true;
    rotateRightArmBackward=false;
    
    rotateLeftArmForward=false;
    rotateLeftArmBackward=true;
    
    rotateRightLegForward=false;
    rotateRightLegBackward=true;
    
    rotateLeftLegForward=true;
    rotateLeftLegBackward=false;
    legCounterR=0; armCounterR=0; legCounterW=0; armCounterW=0;
    legCounterC=0; armCounterC=0; legCounterA=0; armCounterA=0;
    updateModelViews();
}
void emotions (int n){
    if(n==0){
        walking=false;
        running=false;
        isCalm=true;
        isAngry=false;
        movType=Calm;
    }else if(n==1){
        walking=false;
        running=false;
        isCalm=false;
        isAngry=true;
        movType=Angry;
    }
    for(int i=0 ; i<4 ; i++){
        for (int j=0; j<15; j++){
            for(int k=0; k<3; k++){
                Angles[i][j][k] = AnglesConst[i][j][k];
            }
        }
    }
    rotateRightArmForward=true;
    rotateRightArmBackward=false;
    
    rotateLeftArmForward=false;
    rotateLeftArmBackward=true;
    
    rotateRightLegForward=false;
    rotateRightLegBackward=true;
    
    rotateLeftLegForward=true;
    rotateLeftLegBackward=false;
    legCounterR=0; armCounterR=0; legCounterW=0; armCounterW=0;
    legCounterC=0; armCounterC=0; legCounterA=0; armCounterA=0;
    updateModelViews();
}
void displayType(int n){
}

void mainMenu(int n){}

void createMenu(){
    int genderTypeMenu = glutCreateMenu(genderType);
    glutAddMenuEntry("Male", 0);
    glutAddMenuEntry("Female", 1);
    
    int ageMenu = glutCreateMenu(age);
    glutAddMenuEntry("Child", 0);
    glutAddMenuEntry("Adult", 1);
    glutAddMenuEntry("Elder", 2);
    
    int speedMenu = glutCreateMenu(speed);
    glutAddMenuEntry("Walking", 0);
    glutAddMenuEntry("Running", 1);
    
    int emotionMenu = glutCreateMenu(emotions);
    glutAddMenuEntry("Calm", 0);
    glutAddMenuEntry("Angry", 1);
    
    int displayingTypeMenu = glutCreateMenu(displayType);
    glutAddMenuEntry("Nodes", 0);
    glutAddMenuEntry("Lines", 1);
    
    glutCreateMenu(mainMenu);
    glutAddSubMenu("GENDER", genderTypeMenu);
    glutAddSubMenu("AGE", ageMenu);
    glutAddSubMenu("SPEED", speedMenu);
    glutAddSubMenu("EMOTIONS", emotionMenu);
    //glutAddSubMenu("DISPLAYING TYPE", displayingTypeMenu);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode(  GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
    glutInitWindowSize( 512, 512 );
    glutInitWindowPosition( 50, 50 );
    glutCreateWindow( "Body Model Animation" );
    glewExperimental = GL_TRUE;
    glewInit();
    init();
    //-------------------------------callback functions-----------------------------------//
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc(keyboard);
    glutMouseFunc( mouse );
    glutTimerFunc(5,timer,0);
    //------------------------------------------------------------------------------------//
    createMenu();
    glutMainLoop();
    return 0;
}
