#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SWAP(a, b) \
     {             \
          int tmp; \
          tmp = a; \
          a = b;   \
          b = tmp; \
     }

FILE *fp_in, *fp_out;
char *idx;
int *arr, *ans;
int cnt;
/*1*/
int insertion_sort(int left, int right)
{
     for (int i = left + 1; i <= right; i++)
     {
          int key = arr[i];
          int j = i - 1;
          while (j >= 0 && arr[j] > key)
          {
               arr[j + 1] = arr[j];
               j--;
          }
          arr[j + 1] = key;
     }
}
/*2*/
int partition(int left, int right)
{
     int pivot = left;
     int i = left + 1, j = right;
     while (i <= j)
     {
          while (i <= right && arr[i] <= arr[pivot])
               i++;
          while (j > left && arr[j] >= arr[pivot])
               j--;
          if (i > j)
          {
               SWAP(arr[j], arr[pivot]);
          }
          else
          {
               SWAP(arr[i], arr[j]);
          }
     }
     return j;
}
void quick_sort(int left, int right)
{
     int pivot;
     if ((right - left) > 0)
     {
          pivot = partition(left, right);
          quick_sort(left, pivot - 1);
          quick_sort(pivot + 1, right);
     }
}
/*3*/
void merge(left, mid, right)
{
     int i = left, j = mid + 1, k = left;
     while (i <= mid && j <= right)
     {
          if (arr[i] <= arr[j])
          {
               ans[k] = arr[i];
               i++;
          }
          else
          {
               ans[k] = arr[j];
               j++;
          }
          k++;
     }
     while (i <= mid)
     {
          ans[k] = arr[i];
          i++;
          k++;
     }
     while (j <= right)
     {
          ans[k] = arr[j];
          j++;
          k++;
     }
     for (int t = left; t <= right; t++)
     {
          arr[t] = ans[t];
     }
}
void merge_sort(int left, int right)
{
     if (left >= right)
          return;
     int mid = (left + right) / 2;
     merge_sort(left, mid);
     merge_sort(mid + 1, right);
     merge(left, mid, right);
}
/*4*/
void hj_sort(int left, int right)
{
     while (left < right)
     {
          if (right - left + 1 < 30)
          {
               insertion_sort(left, right);
               break;
          }
          else
          {
               SWAP(arr[left], arr[(left + right) / 2]);
               int pivot = partition(left, right);
               if (pivot - left < right - pivot)
               {
                    hj_sort(left, pivot - 1);
                    left = pivot + 1;
               }
               else
               {
                    hj_sort(pivot + 1, right);
                    right = pivot - 1;
               }
          }
     }
}
void print_result()
{
     for (int i = 0; i < cnt; i++)
     {
          fprintf(fp_out, "%d ", arr[i]);
     }
     fprintf(fp_out, "\n");
}

int main(int argc, char *argv[])
{
     /*open input file*/
     fp_in = fopen(argv[1], "r");
     /*make output file*/
     char *str, out[256] = "result_";
     str = argv[1];
     idx = argv[2];
     strcat(out, idx);
     strcat(out, "_");
     strcat(out, str);
     fp_out = fopen(out, "w");
     /*read input file*/
     fscanf(fp_in, "%d ", &cnt);
     arr = (int *)malloc(sizeof(int) * cnt);
     int num;
     for (int i = 0; i < cnt; i++)
     {
          fscanf(fp_in, "%d ", &num);
          arr[i] = num;
     }

     fprintf(fp_out, "%s\n%s\n%d\n", str, idx, cnt);
     if (!strcmp(idx, "1"))
     {
          clock_t start, end;
          start = (double)clock();
          insertion_sort(0, cnt - 1);
          end = (double)clock();
          fprintf(fp_out, "%lf\n", (double)(end - start) / CLOCKS_PER_SEC);
          print_result();
     }
     else if (!strcmp(idx, "2"))
     {
          clock_t start, end;
          start = (double)clock();
          quick_sort(0, cnt - 1);
          end = (double)clock();
          fprintf(fp_out, "%lf\n", (double)(end - start) / CLOCKS_PER_SEC);
          print_result();
     }
     else if (!strcmp(idx, "3"))
     {
          ans = (int *)malloc(sizeof(int) * cnt);
          clock_t start, end;
          start = (double)clock();
          merge_sort(0, cnt - 1);
          end = (double)clock();
          fprintf(fp_out, "%lf\n", (double)(end - start) / CLOCKS_PER_SEC);
          print_result();
     }
     else if (!strcmp(idx, "4"))
     {
          clock_t start, end;
          start = (double)clock();
          hj_sort(0, cnt - 1);
          end = (double)clock();
          fprintf(fp_out, "%lf\n", (double)(end - start) / CLOCKS_PER_SEC);
          print_result();
     }

     // end
     free(arr);
     fclose(fp_in);
     fclose(fp_out);
}
