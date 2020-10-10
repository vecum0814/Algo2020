#include <stdlib.h> // atoi, rand, malloc, realloc
#include <stdio.h>
#include <time.h> //time

#define RANGE 10000

typedef struct
{
	int x;
	int y;
} t_point;

typedef struct
{
	t_point from;
	t_point to;
} t_line;

////////////////////////////////////////////////////////////////////////////////
void print_header( char *filename)
{
	printf( "#! /usr/bin/env Rscript\n");
	printf( "png(\"%s\", width=700, height=700)\n", filename);
	
	printf( "plot(1:%d, 1:%d, type=\"n\")\n", RANGE, RANGE);
}
////////////////////////////////////////////////////////////////////////////////
void print_footer( void)
{
	printf( "dev.off()\n");
}

////////////////////////////////////////////////////////////////////////////////
/*
#points
points(2247,7459)
points(616,2904)
points(5976,6539)
points(1246,8191)
*/
void print_points( t_point *points, int num_point){
    for(int i = 0; i < num_point; i++){
        printf("points(%d,%d)\n",points[i].x, points[i].y);
    }
}

/*
#line segments
segments(7107,2909,7107,2909)
segments(43,8,5,38)
segments(43,8,329,2)
segments(5047,8014,5047,8014)
*/
void print_line_segments( t_line *lines, int num_line){
    for(int i = 0; i < num_line; i++){
        printf("segments(%d,%d,%d,%d)\n", lines[i].from.x, lines[i].from.y, lines[i].to.x, lines[i].to.y);
    }
}

// [input] points : set of points
// [input] num_point : number of points
// [output] num_line : number of line segments that forms the convex hull
// return value : set of line segments that forms the convex hull

//점들의 집합, 점의 갯수, 결과로 선이 몇개 만들어졌고 그 선들의 집합이 lines
t_line *convex_hull( t_point *points, int num_point, int *num_line){
    int line_cnt = 0;
    t_line *line_set = (t_line*)malloc(sizeof(t_line) * line_cnt);
    for(int i = 0; i < num_point - 1; i++){
        for(int j = i + 1; j < num_point; j++){
            int a = points[j].y - points[i].y;
            int b = points[i].x - points[j].x;
            int c = (points[i].x * points[j].y) - (points[j].x * points[i].y);
            
            int check = 1;
            int res = 1;
            int cnt;
            for(int k = 0; k < num_point; k++){
                if(k == i || k == j){
                    continue;
                }
                int tmp = (a * points[k].x) + (b * points[k].y) - c;
                if(tmp == 0){
                    continue;
                }
                tmp = tmp > 0 ? 1 : 0;
                if(check){
                    cnt = tmp;
                    check = 0;
                }
                else{
                    if(tmp != cnt){
                        res = 0;
                        break;
                    }
                }
            }
            
            if(res){
                line_cnt += 1;
                line_set = realloc(line_set, sizeof(t_line) * line_cnt);
                line_set[line_cnt - 1].from = points[i];
                line_set[line_cnt - 1].to = points[j];
            }
        }
    }
    *num_line = line_cnt;
    return line_set;
    
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int x, y;
	int num_point; // number of points
	int num_line; // number of lines
	
	if (argc != 2)
	{
		printf( "%s number_of_points\n", argv[0]);
		return 0;
	}

	num_point = atoi( argv[1]);
	if (num_point <= 0)
	{
		printf( "The number of points should be a positive integer!\n");
		return 0;
	}

	t_point *points = (t_point *) malloc( num_point * sizeof( t_point));
	
    //convex_hull 돌려서 나온 결과를 얘가 받아서 사용할거임
	t_line *lines;
    

	// making n points
	srand( time(NULL));
	for (int i = 0; i < num_point; i++)
	{
		x = rand() % RANGE + 1; // 1 ~ RANGE random number
		y = rand() % RANGE + 1;
		
		points[i].x = x;
		points[i].y = y;
 	}

	fprintf( stderr, "%d points created!\n", num_point);

	print_header( "convex.png");
	
	print_points( points, num_point);
	
    //점들의 집합, 점의 갯수, 결과로 선이 몇개 만들어졌고 그 선들의 집합이 lines
	lines = convex_hull( points, num_point, &num_line);

	fprintf( stderr, "%d lines created!\n", num_line);

	print_line_segments( lines, num_line);
		
	print_footer();
	
	free( points);
	free( lines);
	
	return 0;
}
