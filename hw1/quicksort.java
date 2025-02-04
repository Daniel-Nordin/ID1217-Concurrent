import java.util.Arrays;
import java.util.Random;
import java.util.Timer;

public class quicksort {

    public static void parallelQuickSort(int[] array, int left, int right) {
        if (left < right) {
            int pivotIndex = partition(array, left, right);

            // Create new threads for both partitions
            Thread leftSubList = new Thread(() -> parallelQuickSort(array, left, pivotIndex - 1));
            Thread rightSubList = new Thread(() -> parallelQuickSort(array, pivotIndex + 1, right));

            leftSubList.start();
            rightSubList.start();

            try {
                leftSubList.join();
                rightSubList.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    private static int partition(int[] array, int left, int right) {
        int pivot = array[right];
        int i = left - 1;
        for (int j = left; j < right; j++) {
            if (array[j] <= pivot) {
                i++;
                swap(array, i, j);
            }
        }
        swap(array, i + 1, right);
        return i + 1;
    }

    private static void swap(int[] array, int i, int j) {
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }

    public static void main(String[] args) {
        //sets size if given as argument
        //generates a random array of n size
        Integer size = 10;
        if(args.length > 0)
            size = Integer.parseInt(args[0]);
        int[] array = new int[size];
        Random rnd = new Random();
        for (int i = 0; i < size; i++){
            array[i] = rnd.nextInt(100);
        }
        System.out.println("Unsorted: " + Arrays.toString(array));

        //timer starts
        long start = System.nanoTime();

        parallelQuickSort(array, 0, array.length - 1);
        long end = System.nanoTime();
        System.out.println("Sorted: " + Arrays.toString(array));
        System.out.println("Execution time: " + ((end - start)/1000000) + " milli secons");

    }
}
