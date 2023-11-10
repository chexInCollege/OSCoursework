public class Main {
    public static void main(String args[]) {
        Print printRunnable = new Print();
        Thread t1 = new Thread(new PrintNumbers(false, printRunnable));
        Thread t2 = new Thread(new PrintNumbers(true, printRunnable));
        
        t1.start();
		t2.start();
    }
}

class Print {
    private Object lock = new Object();
    private volatile boolean isEven = true;
    
    public void even(int n) throws InterruptedException {
        synchronized(lock) {
            while (isEven) {
                lock.wait();
            }
            System.out.println(n);
            isEven = true;
            lock.notifyAll();
        }
    }
    
    public void odd(int n) throws InterruptedException {
        synchronized(lock) {
            while (!isEven) {
                lock.wait();
            }
            System.out.println(n);
            isEven = false;
            lock.notifyAll();
        }        
    }
}

class PrintNumbers implements Runnable {
    private boolean isEven;
    Print print;
    
    PrintNumbers(boolean isEven, Print p) {
        this.isEven = isEven;
        this.print = p;
    }
    
     public void run() {
        int n;
        if (isEven) {
            n = 2;
        } else {
            n = 1;
        }
        
        while(n <= 20) {
            if(!isEven) {
                try{print.odd(n);} catch (InterruptedException e) {}
            } else {
                try{print.even(n);} catch (InterruptedException e) {}
            }
            n += 2;
        }
     }
}