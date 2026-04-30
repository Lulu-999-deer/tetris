#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<windows.h>

#define width 10
#define height 20
#define deadline 15

//types\rotate\number\xycoordinate
const int shapes[7][4][4][2]={
	//I type
	{
		{{0,0},{1,0},{2,0},{3,0}},//0°
		{{0,0},{0,1},{0,2},{0,3}},//90°
		{{0,0},{1,0},{2,0},{3,0}},//180°
		{{0,0},{0,1},{0,2},{0,3}}//270°	
	},
	//T type
	{
		{{0,0},{1,0},{2,0},{1,1}},//0°,up
		{{0,0},{0,1},{0,2},{1,1}},//90°,right
		{{0,1},{1,1},{2,1},{1,0}},//180°,down
		{{0,1},{1,0},{1,1},{1,2}}//270°,left
	},
	//O type
	{
		{{0,0},{1,0},{0,1},{1,1}},//0°
		{{0,0},{1,0},{0,1},{1,1}},//90°
		{{0,0},{1,0},{0,1},{1,1}},//180°
		{{0,0},{1,0},{0,1},{1,1}}//270°
	},
	//J type
	{
		{{0,0},{0,1},{1,0},{2,0}},//0°,right
		{{0,0},{0,1},{0,2},{1,2}},//90°,down
		{{0,1},{1,1},{2,1},{2,0}},//180°,left
		{{0,0},{1,0},{1,1},{1,2}}//270°,up
	},	
	//L type
	{
		{{0,0},{1,0},{2,0},{2,1}},//0°,left
		{{0,0},{0,1},{0,2},{1,0}},//90°,up
		{{0,0},{0,1},{1,1},{2,1}},//180°,right
		{{0,2},{1,0},{1,1},{1,2}}//270°,down
	},
	//S type
	{
		{{0,0},{1,0},{1,1},{2,1}},//0°
		{{0,1},{0,2},{1,0},{1,1}},//90°
		{{0,0},{1,0},{1,1},{2,1}},//180°
		{{0,1},{0,2},{1,0},{1,1}}//270°
	},
	//Z type
	{
		{{0,1},{1,1},{1,0},{2,0}},//0°
		{{0,0},{0,1},{1,1},{1,2}},//90°
		{{0,1},{1,1},{1,0},{2,0}},//180°
		{{0,0},{0,1},{1,1},{1,2}}//270°
	}
};


int get_shapes(char shapes){
	switch(shapes){
	case 'I':
		return 0;
	case 'T':
		return 1;
	case 'O':
		return 2;
	case 'J':
		return 3;
	case 'L':
		return 4;
	case 'S':
		return 5;
	case 'Z':
		return 6;
	default:
		return -1;
	}
	
}

typedef struct position{
	int shape;
	int rotation;
	int x,y;	
}position;

position now,next;
int score=0;
int occupied[width][height]={0};


int if_collision(position t){
	int i=0;
	for(i=0;i<4;i++){
		int newx,newy;
		newx=t.x + shapes[t.shape][t.rotation][i][0];
		newy=t.y - shapes[t.shape][t.rotation][i][1];
		if(newx<0 || newy<0 || newx>=width || occupied[newx][newy] != 0 )
		return 1;	
	}
	return 0;
}

int full_lines(int line){
	int x=0;
	for(x=0;x<width;x++){
		if(occupied[x][line]==0){
			return 0;
		}
	}
	return 1;
}


void clear_lines(){
	int num_cleared=0;
	int y=height-1;
	for(y=height-1;y>=0;y--){
		if(full_lines(y)){
			num_cleared++;
		for (int i = y; i > 0; i--) {
			for (int j = 0; j < width; j++) {
				occupied[j][i] = occupied[j][i-1];}
		}
		for (int j = 0; j < width; j++) {
			occupied[j][0] = 0; }// Clear the top line after shifting
		y++;
		}	
	}
	
	if(num_cleared==1){
		score+=100;
	}
	else if(num_cleared==2){
		score+=300;
	}
	else if(num_cleared==3){
		score+=500;
	}
	else if(num_cleared==4){
		score+=800;
	}	
}

int rotate(){
	position temp = now;
	temp.rotation = (temp.rotation + 1) % 4; // 顺时针旋转
	if (!if_collision(temp)) {
		now.rotation = temp.rotation;
		return 1; // 旋转成功
	}
	return 0;
}

int lock(){
	int i;
	for (i = 0; i < 4; i++) {
		int newx = now.x + shapes[now.shape][now.rotation][i][0];
		int newy = now.y - shapes[now.shape][now.rotation][i][1];
		if (newx < 0 || newx >= width || newy < 0 || newy >= height) {
			return 0; }
		occupied[newx][newy] = 1;
	}
	clear_lines();
	return 1;
}

int find_lowest_position(position t) {
	int lowest_y = height;
	for (int i = 0; i < 4; i++) {
		int newx = t.x + shapes[t.shape][t.rotation][i][0];
		int newy = t.y - shapes[t.shape][t.rotation][i][1];
		if (newy < lowest_y) {
			lowest_y = newy;
		}
	}
	return lowest_y;
}


void drop_to_bottom() {
	position temp = now;
	while (1) {
		temp.y--;
		if (if_collision(temp)) {
			temp.y++;
			break;
		}
	}
	now.y = temp.y;
}



int main() {
   int main() {
	   char input[3];
	   scanf("%s", input); // 读取初始两个方块的形状
	   now.shape = get_shapes(input[0]);
	   next.shape = get_shapes(input[1]);
	   now.rotation = 0;
	   now.x = width / 2 - 1; // 初始位置在中间偏左
	   now.y = height - 1; // 初始位置在最顶部
	   
	   while (1) {
		   // 尝试放置当前方块
		   drop_to_bottom(); // 尝试下落到最低位置
		   if (!lock()) { // 如果放置失败，游戏结束
			   printf("E\n");
			   exit(0);
		   }
		   
		   // 输出当前方块的放置方案
		   printf("%d %d\n%d\n", now.rotation * 90, now.x, score);
		   fflush(stdout); // 刷新输出缓冲区
		   
		   // 读取下一个方块的形状
		   scanf(" %c", &input[0]); // 注意前面的空格，用于跳过换行符
		   if (input[0] == 'X') { // 如果是最后一个方块，结束程序
			   break;
		   } else if (input[0] == 'E') { // 如果游戏结束，退出程序
			   exit(0);
		   } else {
			   now.shape = next.shape; // 更新当前方块为下一个方块
			   now.rotation = 0;
			   now.x = width / 2 - 1; // 重置初始位置
			   now.y = height - 1;
			   next.shape = get_shapes(input[0]); // 读取新的下一个方块形状
		   }
		   
		   // 检查是否达到死亡判定线
		   for (int y = deadline; y < height; y++) {
			   for (int x = 0; x < width; x++) {
				   if (occupied[x][y]) {
					   printf("E\n");
					   exit(0);
				   }
			   }
		   }
	   }
	   return 0;
   } 
}


