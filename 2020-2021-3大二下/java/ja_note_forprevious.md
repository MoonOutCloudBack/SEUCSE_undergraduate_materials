

## 简答

### ArrayList和LinkedList的区别

arraylist是线性表+动态数组，linkedlist顾名思义是链表。

arraylist占用空间更多更灵活，linkedlist占用空间更少。

arraylist可以实现高效的随机访问但linkedlist不行。



### 线程生命周期几种状态，java控制线程状态的几种方法

![image-20210629101629845](.\..\..\typora-user-images\image-20210629101629845.png)



### 抽象类和接口的异同

![image-20210629101738139](.\..\..\typora-user-images\image-20210629101738139.png)



### 写SQL语句

这个分数大概可以拿到。score+=10。

```sql
SELECT COUNT(DISTINCT Vendor) FROM BookTable;
SELECT Price FROM BookTable WHERE Vendor ="高教出版社";
INSERT INTO BookTable (ID,Name,Vendor,Year,Price) VALUES (6,'数学分析','东南大学出版社',2018,85);
DELETE FROM BookTable WHERE Name LIKE '%数%';
UPDATE BookTable SET Price=100 WHERE YEAR>=2005;
```

```sql
SELECT * FROM tablename;
SELECT COUNT(DISTINCT xxx) FROM tablename WHERE xxx LIKE '%正则表达式%';
SELECT xxx FROM tablename WHERE xxx BETWEEN lower AND upper ORDER BY xxx_for_sort DESC / ASC

INSERT INTO tablename (c1,c2,c3) VALUES (v1,v2,v3);
DELETE FROM tablename WHERE xxx in ('xxx1','xxx2');
UPDATE tablename SET xxx=newxxx WHERE xxx='needmodify'；
```



## 编程题

### 多线程简单demo

```java
class task implements Runnable{
    @Override
    public void run(){
        while(true){
            System.out.println(Thread.currentThread().getName());
            sleep(1000);
        }
    }
    
    public static void main(String[] args){
        Thread t1=new Thread(new task(),"Thread One");
        ...;
        t1.start();
        ...;
    }
}
```

### 网络编程

正在考虑是否放弃最后20分……



























