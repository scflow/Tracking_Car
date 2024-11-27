#include "zf_device_mt9v03x.h"
#include "image_processing.h"
#include "search_midline.h"

//struct LEFT_EDGE  L_edge[120];     //��߽�ṹ��
//struct RIGHT_EDGE R_edge[120];    //�ұ߽�ṹ��

uint8 L_edge_count = 0, R_edge_count = 0;                       //���ұߵ�ĸ���
uint8 dire_left, dire_right;                                    //��¼��һ��������λ��
uint8 L_search_amount = 140, R_search_amount = 140;             //���ұ߽��ѵ�ʱ�������ĵ�
uint8 Boundary_search_end = MT9V03X_H;
uint8 L_start_x,L_start_y,left_lose;
uint8 R_start_x,R_start_y,right_lose;
uint8 left_findflag, right_findflag;

// �������     �յ���
uint8 L_corner_flag = 0;//��յ���ڱ�־
uint8 L_corner_row[5];//��յ�������
uint8 L_corner_col[5];//��յ�������
int L_corner_angle = 0;//��յ�Ƕ�
uint8 R_corner_flag = 0;//�ҹյ���ڱ�־
uint8 R_corner_row[5];//�ҹյ�������
uint8 R_corner_col[5];//�ҹյ�������
int R_corner_angle = 0;//�ҹյ�Ƕ�
uint8 enable_L_corner = 1, enable_R_corner = 1;
uint8 Mid_line[MT9V03X_H];
uint8 Left_edge[MT9V03X_H];
uint8 Right_edge[MT9V03X_H];

float error,last_error, YERR;
uint8 far,mid,near;

struct LEFT_EDGE
{
    uint8 row;  //������
    uint8 col;  //������
    uint8 flag; //���ڱ߽�ı�־
}L_edge[120];
struct RIGHT_EDGE
{
    uint8 row;  //������
    uint8 col;  //������
    uint8 flag; //���ڱ߽�ı�־
}R_edge[120];
//---------------------------------------------------------------
// �������     ������߽��ʼ��
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     clear_find_point();
//---------------------------------------------------------------
void clear_find_point(void)
{
    for (int i = 0;i < 140;i++)
    {
        L_edge[i].row = 0;
        L_edge[i].col = 0;
        L_edge[i].flag = 0;
    }
    for (int i = 0;i < 140;i++)
    {
        R_edge[i].row = 0;
        R_edge[i].col = 0;
        R_edge[i].flag = 0;
    }
}

//---------------------------------------------------------------
// �������     �������ұ߽�
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     search_neighborhood();
//---------------------------------------------------------------

void search_neighborhood(void)
{
    L_edge_count = 0;//��ߵ������0
    R_edge_count = 0;//�ұߵ������0

    if (left_findflag)//�����߽����ڲ��ҵ�,��ʼ����
    {
        //��������
        L_edge[0].row = L_start_y;
        L_edge[0].col = L_start_x;
        L_edge[0].flag = 1;
        uint8 curr_row = L_start_y;//��ʼ��������
        uint8 curr_col = L_start_x;//��ʼ��������
        dire_left = 0; //��ʼ���ϸ��߽�������
        //��ʼ���ߣ����ȡ140���㣬���������ѣ���7����λ
        for (int i = 1;i < L_search_amount; i++)    //�������140����
        {
            //Խ���˳� ��Խ�����Խ�磨���������������ң�
            if (curr_row + 1 < Boundary_search_end || curr_row > MT9V03X_H - 1)  break;
            //���߹���
            if (dire_left != 2 && Image_use[curr_row - 1][curr_col - 1] == BLACK && Image_use[curr_row - 1][curr_col] == WHITE)   //���Ϻڣ�2���ұ߰�
            {
                curr_row = curr_row - 1;
                curr_col = curr_col - 1;
                L_edge_count = L_edge_count + 1;
                dire_left = 7;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
            }
            else if (dire_left != 3 && Image_use[curr_row - 1][curr_col + 1] == BLACK && Image_use[curr_row][curr_col + 1] == WHITE)    //���Ϻڣ�3���±߰�
            {
                curr_row = curr_row - 1;
                curr_col = curr_col + 1;
                L_edge_count = L_edge_count + 1;
                dire_left = 6;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
            }
            else if (Image_use[curr_row - 1][curr_col] == BLACK && Image_use[curr_row - 1][curr_col + 1] == WHITE)                  //���Ϻڣ�1���Ұ�
            {
                curr_row = curr_row - 1;
                L_edge_count = L_edge_count + 1;
                dire_left = 0;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
            }
            else if (dire_left != 5 && Image_use[curr_row][curr_col - 1] == BLACK && Image_use[curr_row - 1][curr_col - 1] == WHITE)     //����ڣ�5���ϰ�
            {
                curr_col = curr_col - 1;
                L_edge_count = L_edge_count + 1;
                dire_left = 4;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
            }
            else if (dire_left != 4 && Image_use[curr_row][curr_col + 1] == BLACK && Image_use[curr_row + 1][curr_col + 1] == WHITE)  //���Һڣ�4���°�
            {
                curr_col = curr_col + 1;
                L_edge_count = L_edge_count + 1;
                dire_left = 5;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
            }
            else if (dire_left != 6 && Image_use[curr_row + 1][curr_col - 1] == BLACK && Image_use[curr_row][curr_col - 1] == WHITE)    //���ºڣ�6���ϰ�
            {
                curr_row = curr_row + 1;
                curr_col = curr_col - 1;
                L_edge_count = L_edge_count + 1;
                dire_left = 3;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
            }
            else if (dire_left != 7 && Image_use[curr_row + 1][curr_col + 1] == BLACK && Image_use[curr_row + 1][curr_col] == WHITE)    //���ºڣ�7�����
            {
                curr_row = curr_row + 1;
                curr_col = curr_col + 1;
                L_edge_count = L_edge_count + 1;
                dire_left = 2;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
            }
            else
                break;
        }
    }

    if (right_findflag)//����ұ߽���ڲ��ѵ�
    {
        R_edge[0].row = R_start_y;
        R_edge[0].col = R_start_x;
        R_edge[0].flag = 1;
        uint8 curr_row = R_start_y;
        uint8 curr_col = R_start_x;
        dire_right = 0;
        for (int i = 1;i < R_search_amount;i++)
        {
            //Խ���˳� ��Խ�����Խ�磨���������������ң�
            if (curr_row < Boundary_search_end || curr_row>IMAGE_H - 1 || curr_row + 1 < Boundary_search_end)  break;
            //���߹���
            if (curr_col < IMAGE_W && dire_right != 3 && Image_use[curr_row - 1][curr_col + 1] == BLACK && Image_use[curr_row - 1][curr_col] == WHITE)    //���Ϻڣ�3�����
            {
                curr_row = curr_row - 1;
                curr_col = curr_col + 1;
                R_edge_count = R_edge_count + 1;
                dire_right = 6;
                R_edge[i].row = curr_row;
                R_edge[i].col = curr_col;
                R_edge[i].flag = 1;
            }
            else if (dire_right != 2 && Image_use[curr_row - 1][curr_col - 1] == BLACK && Image_use[curr_row][curr_col - 1] == WHITE) //���Ϻڣ�2���°�
            {
                curr_row = curr_row - 1;
                curr_col = curr_col - 1;
                R_edge_count = R_edge_count + 1;
                dire_right = 7;
                R_edge[i].row = curr_row;
                R_edge[i].col = curr_col;
                R_edge[i].flag = 1;
            }
            else if (Image_use[curr_row - 1][curr_col] == BLACK && Image_use[curr_row - 1][curr_col - 1] == WHITE)                  //���Ϻڣ�1�����
            {
                curr_row = curr_row - 1;
                R_edge_count = R_edge_count + 1;
                dire_right = 0;
                R_edge[i].row = curr_row;
                R_edge[i].col = curr_col;
                R_edge[i].flag = 1;
            }
            else if (dire_right != 4 && Image_use[curr_row][curr_col + 1] == BLACK && Image_use[curr_row - 1][curr_col + 1] == WHITE)   //���Һڣ�4���ϰ�
            {
                curr_col = curr_col + 1;
                R_edge_count = R_edge_count + 1;
                dire_right = 5;
                R_edge[i].row = curr_row;
                R_edge[i].col = curr_col;
                R_edge[i].flag = 1;
            }
            else if (dire_right != 5 && Image_use[curr_row][curr_col - 1] == BLACK && Image_use[curr_row + 1][curr_col - 1] == WHITE)   //����ڣ�5���°�
            {
                curr_col = curr_col - 1;
                R_edge_count = R_edge_count + 1;
                dire_right = 4;
                R_edge[i].row = curr_row;
                R_edge[i].col = curr_col;
                R_edge[i].flag = 1;
            }
            else if (dire_right != 6 && Image_use[curr_row + 1][curr_col - 1] == BLACK && Image_use[curr_row + 1][curr_col] == WHITE)   //���ºڣ�6���Ұ�
            {
                curr_row = curr_row + 1;
                curr_col = curr_col - 1;
                R_edge_count = R_edge_count + 1;
                dire_right = 3;
                R_edge[i].row = curr_row;
                R_edge[i].col = curr_col;
                R_edge[i].flag = 1;
            }
            else if (dire_right != 7 && Image_use[curr_row + 1][curr_col + 1] == BLACK && Image_use[curr_row][curr_col + 1] == WHITE)   //���ºڣ�7���ϰ�
            {
                curr_row = curr_row + 1;
                curr_col = curr_col + 1;
                R_edge_count = R_edge_count + 1;
                dire_right = 2;
                R_edge[i].row = curr_row;
                R_edge[i].col = curr_col;
                R_edge[i].flag = 1;
            }
            else
                break;
        }
    }
}


//---------------------------------------------------------------
// �������     ��Ⱥ�
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��
// ע������     Լ�Ŵ�128��
//---------------------------------------------------------------
int16 calc_diff(int16 x, int16 y)
{
    return (((x - y) << 7) / (x + y));
}

//---------------------------------------------------------------
// �������      �޷�
// ����˵��      x               ���޷�������
// ����˵��      y               �޷���Χ(���ݻᱻ������-y��+y֮��)
// ���ز���      float           �޷�֮�������
// ʹ��ʾ��      float dat = limit(500,300);//���ݱ�������-300��+300֮��  ��˷��صĽ����300
//---------------------------------------------------------------
float limit(float x, int32 y)
{
    if (x > y)             return (float)y;
    else if (x < -y)       return (float)(-y);
    else                   return x;
}

//---------------------------------------------------------------
// �������     �յ���
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     get_turning_point();
//---------------------------------------------------------------

void get_turning_point(void)
{

    L_corner_flag = 0;// ��ʼ������
    L_corner_angle = 0;
    if (enable_L_corner) //���ʹ��������յ�
    {
        uint8 k = 0;
        if (L_edge_count > 9 && L_start_y >= IMAGE_H / 2 && left_lose <= IMAGE_H / 2)
        {
            for (int i = 0; i < L_edge_count - 9;i++)
            {
                if (L_edge[i + 8].row > 5)
                {
                    if ((L_edge[i].col - L_edge[i + 4].col) * (L_edge[i + 8].col - L_edge[i + 4].col) +
                        (L_edge[i].row - L_edge[i + 4].row) * (L_edge[i + 8].row - L_edge[i + 4].row) >= 0) //����ȷ��Ϊ��ǻ���ֱ�� ������
                    {
                        L_corner_angle = Get_angle(L_edge[i].col, L_edge[i].row, L_edge[i + 4].col, L_edge[i + 4].row, L_edge[i + 8].col, L_edge[i + 8].row); //��Ƕ�
                        if (L_edge[i + 4].col > L_edge[i + 8].col && L_corner_angle >= 28 && L_corner_angle <= 110)
                        {
                            L_corner_flag = 1;
                            L_corner_row[k] = L_edge[i + 4].row;
                            L_corner_col[k] = L_edge[i + 4].col;
                            k++;
                        }
                    }
                }
            }
        }
    }
    R_corner_flag = 0;//��ʼ������
    R_corner_angle = 0;
    if (enable_R_corner)    //���ʹ�������ҹյ�
    {
        uint8 k = 0;
        if (R_edge_count > 9 && R_start_y >= IMAGE_H / 2 && right_lose <= IMAGE_H / 2)
        {
            for (int i = 0; i < R_edge_count - 9;i++)
            {
                if (R_edge[i + 8].row > 5)
                {
                    if ((R_edge[i].col - R_edge[i + 4].col) * (R_edge[i + 8].col - R_edge[i + 4].col) +
                        (R_edge[i].row - R_edge[i + 4].row) * (R_edge[i + 8].row - R_edge[i + 4].row) >= 0) //����ȷ��Ϊ��ǻ���ֱ�� ������
                    {
                        R_corner_angle = Get_angle(R_edge[i].col, R_edge[i].row, R_edge[i + 4].col, R_edge[i + 4].row, R_edge[i + 8].col, R_edge[i + 8].row); //��Ƕ�
                        if (R_edge[i + 8].col > R_edge[i + 4].col && R_corner_angle >= 28 && R_corner_angle <= 110)
                        {
                            R_corner_flag = 1;
                            R_corner_row[k] = R_edge[i + 4].row;
                            R_corner_col[k] = R_edge[i + 4].col;
                            k++;
                        }
                    }
                }
            }
        }
    }
}

//-----------------------------------------------------
//  �������     ����
//  ����˵��     void
//  ���ز���     void
//  ʹ��ʾ��     ();
//-----------------------------------------------------


//-----------------------------------------------------
//  �������     ����Ѳ�ߺ�����������
//  ����˵��     void
//  ���ز���     void
//  ʹ��ʾ��     Horizontal_line();
//-----------------------------------------------------
void Horizontal_line(void)
{
    uint8 i, j;
    for(i=0;i<120;i++)
    {
        Mid_line[i] = 93;
    }
    for(i=Mid_line[119];i>=1;i--){//�ҵ�������
        if(Image_use[119][i] - Image_use[119][i-1] == 255){
            Left_edge[119] = i ;
        }
    }
    for(i=Mid_line[119];i<=186;i++){//�ҵ�������
        if(Image_use[119][i] - Image_use[119][i+1] == 255){
            Right_edge[119] = i ;
        }
    }
    Mid_line[119] = (Left_edge[119]+Right_edge[119])/2;//�����һ������
    for(i=118;i>0;i--){
        for(j = Mid_line[i+1];j>=1;j--){
        if(Image_use[i][j]-Image_use[i][j-1] == 255){
            Left_edge[i] = j;//������
          }//�ҵ��ϱ���
        }
        for(j=Mid_line[i+1];j<=186;j++);
            if(Image_use[i][j]-Image_use[i][j+1] == 255){
                Right_edge[i] = j;//������
          }//�ҵ��ϱ���
        }
    Mid_line[i] = (Left_edge[i]+Right_edge[i])/2;
    }
//-----------------------------------------------------
//  �������     ������
//  ����˵��     void
//  ���ز���     void
//  ʹ��ʾ��     midline();
//-----------------------------------------------------
void midline(void)
{
    uint8 start_scan = 94;
    uint8 i = 0,j = 0;
    for(i=0;i<120;i++)
    {
        Mid_line[i] = 93;
    }
    for(i=MT9V03X_H-1;i>=2;i--)
    {
        //��������ɨ��
        for(j=start_scan;j>=1;j--)//����߽�
        {
            if(Image_use[i][j-1]==0 && Image_use[i][j]==255)
            {
                Left_edge[i] = j;
            }
        }
        for(j=start_scan;j<=186;j++)//���ұ߽�
        {
            if(Image_use[i][j]==255 && Image_use[i][j+1]==0)
            {
                Right_edge[i] = j;
            }
        }
        Mid_line[i] = (Left_edge[i]+Right_edge[i])/2;
        start_scan = Mid_line[i];//��ɨ����ʼ��Ϊ����
    }
}
//-----------------------------------------------------
//  �������     ��͸��֪�������γɵĽǶ�(�����Ͻ�Ϊԭ���γ�����ϵ)
//  ����˵��     Ax��Ay      �±ߵ�
//  ����˵��     Bx��By      Ҫ��Ƕȵ�һ��
//  ����˵��     Cx��Cy      �ϱߵ�
//  ���ز���     void
//  ʹ��ʾ��     get_turning_point();
//-----------------------------------------------------
float hd[3][3] = { {-0.436025, 0.000000, 21.365217}, {0.424457, 0.487500, -22.048370}, {0.018944, -0.000000, 0.071739} };
//x=[18,18,69,69]  % ����ΪA��B��D��C������ͷ��ȡ����Ƭ�еĵ�������
//y=[70,121,70,121] % ����ΪA��B��D��C������ͷ��ȡ����Ƭ�еĵĺ�����
float Get_angle(float Ax, float Ay, float Bx, float By, float Cx, float Cy)
{

    float BA = 0.00;//����BA��ģ
    float BC = 0.00;
    float SBA_BC = 0.00;//������˵�ֵ
    float angle = 0.00;

    float AX = ((hd[0][0] * Ax + hd[0][1] * Ay + hd[0][2]) / (hd[2][0] * Ax + hd[2][1] * Ay + hd[2][2]));
    float AY = ((hd[1][0] * Ax + hd[1][1] * Ay + hd[1][2]) / (hd[2][0] * Ax + hd[2][1] * Ay + hd[2][2]));
    float BX = ((hd[0][0] * Bx + hd[0][1] * By + hd[0][2]) / (hd[2][0] * Bx + hd[2][1] * By + hd[2][2]));
    float BY = ((hd[1][0] * Bx + hd[1][1] * By + hd[1][2]) / (hd[2][0] * Bx + hd[2][1] * By + hd[2][2]));
    float CX = ((hd[0][0] * Cx + hd[0][1] * Cy + hd[0][2]) / (hd[2][0] * Cx + hd[2][1] * Cy + hd[2][2]));
    float CY = ((hd[1][0] * Cx + hd[1][1] * Cy + hd[1][2]) / (hd[2][0] * Cx + hd[2][1] * Cy + hd[2][2]));

    BA = sqrt((AX - BX) * (AX - BX) + (AY - BY) * (AY - BY));
    BC = sqrt((CX - BX) * (CX - BX) + (CY - BY) * (CY - BY));

    SBA_BC = (AX - BX) * (CX - BX) + (AY - BY) * (CY - BY);

    angle = acos(SBA_BC * 1.00 / (BA * BC));

    return angle * 57.3;
}

//-----------------------------------------------------
//  �������     ��ƫ��
//  ����˵��     void
//  ���ز���     void
//  ʹ��ʾ��     get_error();
//-----------------------------------------------------
void get_error(void)
{
/*   error = 0;
   uint8 i = 0;
   float row_sum = 0;
   float error_sum = 0;
   for(i = 65; i > 50; i--)
   {
       error_sum += Mid_line[i] - Mid_road;
      row_sum++;
   }
   error = error_sum/row_sum;
    uint8 distance = 15;  //����ͷǰհ
*/

    error = 0; //ǰհƫ��
    YERR = 0; //�������ƫ��

    near = Mid_line[115];
    mid = Mid_line[100];
    far = Mid_line[85];

    if(far < mid && mid < near)   //���1
    {
        error = -((mid - far) + (near - mid))/2;  //��ȡǰհƫ�� �� ��
    }
    else if(far < mid && mid >= near)//���2
    {
        error = near - mid;   //��ȡǰհƫ��      �� ��
    }
    else if(far >= mid && mid < near)//���3  �� ��
    {
        error = near - mid;   //��ȡǰհƫ��
    }
    else //���4
    {
        error = -((mid - far) + (near - mid))/2;  //��ȡǰհƫ��  �� ��
    }
    YERR = near - 94; //��ȡ�������ƫ��

}
