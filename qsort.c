#include<stdio.h>


int partition(int arr[], int l, int r)
{
    int count = 0, temp, i = 0, j = r, pivot = l;
    for(i = l + 1 ; i <= r ; i++)
        if(arr[i] <= arr[pivot])
            count++;
    temp = arr[l + count];
    arr[l + count] = arr[l];
    arr[l] = temp;
    pivot = count + l;
    i = l;
    j = r;
    while(i != pivot && j != pivot)
    {
        if(arr[i] <= arr[pivot])
            i++;
        else if(arr[j] > arr[pivot])
            j--;
        else
        {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
        
    }
    return pivot;

}
//Quick Sort
void sort(int arr[], int l, int r)
{
    if(l < r)
    {
        int q = partition(arr, l, r);
        sort(arr, l, q - 1);
        sort(arr, q + 1, r);
    }
}

int main()
{
    int arr[100], n;
    scanf("%d", &n);
    for(int i = 0 ; i < n ; i++)
        scanf("%d",&arr[i]);
    
    sort(arr, 0, n - 1);
    for(int i = 0 ; i < n ; i++)
        printf("%d", arr[i]);
    return 0;  
}