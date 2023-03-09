# 一、MySQL基础

1. 主键、外键、索引

主键：不允许重复、不允许为空，为了保证数据完整性。

外键：另一张表的主键，允许重复、可以为空，用于和其他表建立联系（比如班级表，学生表）。

索引：没有重复值，但可以为空，用于提高查询的速度。建立索引的方法有：

（1）第一种：`CREATE INDEX index_name on table_name(columnName);`

（2）第二种：`ALTER TABLE table_name ADD INDEX index_name (columnName);`

2. 索引类型

（1）普通索引

 最基本的索引，它没有任何限制.

CREATE index 索引名 on 表名(列名)

（2）唯一索引

 与前面的普通索引类似，不同的就是：索引列的值必须唯一，但允许有空值。如果是组合索引，则列值的组合必须唯一。

CREATE UNIQUE index 索引名 on 表名(列名)

（3）主键索引

 是一种特殊的唯一索引，这个时候需要一个表只能有一个主键，不允许有空值。一般是在建表的时候同时创建主键索引.也就是说主键约束默认索引

（4）复合索引

 指多个字段上创建的索引，只有在查询条件中使用了创建索引时的第一个字段，索引才会被使用。使用组合索引时遵循最左前缀规格

CREATE index 索引名 on 表名(列名,列名...)

（5）全文索引

 主要用来查找文本中的关键字，而不是直接与索引中的值相比较。fulltext索引跟其它索引大不相同，它更像是一个搜索引擎，而不是简单的where语句的参数匹配。目前只有char、varchar，text 列上可以创建全文索引。值得一提的是，在数据量较大时候，先将数据放入一个没有全局索引的表中，然后再用CREATE index创建fulltext索引，要比先为一张表建立fulltext然后再将数据写入的速度快很多。

CREATE TABLE `info` (
    `id` int(11) NOT NULL AUTO_INCREMENT ,
    `title` char(255) CHARACTER NOT NULL ,
    `content` text CHARACTER NULL ,
    `time` int(10) NULL DEFAULT NULL ,
    PRIMARY KEY (`id`),
    FULLTEXT (content)
);

3. 数据库的基本操作

```mysql
#查看用户权限信息
SELECT USER,HOST FROM mysql.user;    #查看mysql有哪些用户,对应的主机名是什么
SHOW GRANTS FOR root@'localhost';  # 查看root这个用户在本机有哪些权限
-----------------------------------------------------
 
#创建用户 :CREATE USER
CREATE USER test@localhost; #创建一个test的用户,这个用户只能在本机登录
CREATE USER test@localhost IDENTIFIED BY '123456';  #指定密码
-----------------------------------------------------------------
 
#删除用户  :DROP USER
DROP USER test@localhost;
 
----------------------------------------------------
#添加权限  :GRANT 什么权限 ON 所有数据库.所有表  TO  用户名字 (WITH GRANT OPTION :可以把自己的权限赋给别的用户)
GRANT ALL PRIVILEGES ON '*' TO test@localhost;
GRANT ALL PRIVILEGES ON '*' TO test@localhost WITH GRANT OPTION;
GRANT ALL PRIVILEGES ON '*' TO test@localhost IDENTIFIED BY'123456'WITH GRANT OPTION;
 
#添加更新studentno字段的权限,在myschool.student表里,给test这个用户
GRANT UPDATE(studentno) ON myschool.student TO test@localhost IDENTIFIED BY '123456' WITH GRANT OPTION;
 
#添加查询权限,只能查询myschool数据库里面的student表,给zhangsan这个用户
GRANT SELECT ON myschool.student TO zhangsan@localhost WITH GRANT OPTION;
 
 
-------------------------------------------------------------
#删除/收回权限
格式:REVOKE 要移除的权限名字 在哪个数据库哪个表 从哪个用户移除
REVOKE ALL PRIVILEGES ON '*' FROM test@localhost; 

-------------------------------------------------------------
#创建表
CREATE TABLE `表名`(
    `字段名1` 列类型[字段属性|约束] [索引] [注释],
    `字段名2` 列类型[字段属性|约束] [索引] [注释],
    ... ...
    `字段名n` 列类型[字段属性|约束] [索引] [注释]
) [表类型] [表字符集] [注释] ;
 
ps:  [] 中的内容选择性添加 , 多个字段之间使用 逗号分隔 ,最后一个字段无需添加逗号

CREATE TABLE `student`(
	studentno INT(4) UNSIGNED NOT NULL COMMENT '学号',
	loginpwd VARCHAR(20) NOT NULL COMMENT '密码',
	studentname VARCHAR(50) NOT NULL COMMENT '学生姓名',
	sex CHAR(1) NOT NULL DEFAULT '男' COMMENT'性别',
	gradeid INT(4) UNSIGNED COMMENT '年纪编号',
	phone VARCHAR(50) COMMENT '联系电话',
	address VARCHAR(255) DEFAULT '地址不详' COMMENT '家庭住址',
	borndate DATETIME COMMENT '出生日期',
	email VARCHAR(50) COMMENT '邮箱账号',
	identitycard VARCHAR(18) COMMENT '身份证号'
);

-------------------------------------------------------------
#清空表
TRUNCATE `student`

修改表名 : ALTER TABLE 旧表名 RENAME AS 新表名
添加字段 : ALTER TABLE 表名 ADD 字段名 列类型[属性]
修改字段 : ALTER TABLE 表名 MODIFY 字段名 列类型[属性]  #修改列的数据类型
          ALTER TABLE 表名 CHANGE 旧字段名 新字段名 列类型[属性] # 修改列的数据类型以及名称
删除字段 : ALTER TABLE 表名 OROP 字段名

#插入单行数据
INSERT INTO 表名 (字段1,字段2,字段3,...) VALUES (值1,值2,值3,...);

#插入多行数据
INSERT INTO 表名 (字段1,字段2,字段3,...) VALUES 
    (值1,值2,值3,...),
    (值1,值2,值3,...),
    (值1,值2,值3,...),
    ....;

#删除数据
DELETE FROM 表名 [WHERE 条件]

#查询数据
 SELECT * FROM 表名    #查询全部 ,效率低下,不推荐使用
 
 SELECT 字段1,字段2 FROM 表名    #查询指定字段

#查询去重
SELECT DISTINCT  name FROM student;  #去掉重复名字,保证每个名字仅返回一条

#where条件和Between and 范围查询
SELECT 字段1,字段2,... FROM 表名 WHERE 字段x BETWEEN 值1 AND 值2

#like模糊查询
例1: 找出名字中含有  o  的英文名:
SELECT name FROM student WHERE name LIKE '%o%'  ; 
 
例2: 找出名字以  T  结尾的英文名:
SELECT name FROM student WHERE name LIKE '%T'  ; 
 
例3: 找出名字以 K 开始的英文名:
SELECT name FROM student WHERE name LIKE 'K%'  ; 
 
例4: 找出第二个字母是 A 的英文名:
SELECT name FROM student WHERE name LIKE '_A%'  ; 
 
例5: 找出第三个字母是 R 的英文名:
SELECT name FROM student WHERE name LIKE '__R%'  ;   //两个下划线

#in查询
SELECT 字段1,字段2,... FROM 表名 WHERE 字段x IN (值1,值2,值3,...)

#聚合函数查询
#查询学生的总数。
SELECT COUNT(studentname) FROM student; 
#查询课程编号为1的平均分。
SELECT AVG(studentresult) FROM result WHERE subjectno = 1;
#查询课程编号为1的最高分。
SELECT MAX(studentresult) FROM result WHERE subjectno = 1;
#查询课程编号为1的最低分。
SELECT MIN(studentresult) FROM result WHERE subjectno = 1;
#查询课程编号为1的成绩总和。
SELECT SUM(studentresult) FROM result WHERE subjectno = 1;

#排序语句查询
ORDER BY name DESC；

#限制返回数量LIMIT
select * from tableName limit [i] , n;
 
注:
    tableName : 数据表
    i : 为查询结果的索引值 (默认从0开始)   //可以省略
    n : 为查询结果返回的数量

#分组查询
SELECT job, sum(sal)  FROM emp  GROUP BY job;       
#GROUP BY job : 按照工作岗位的不同,分为不同的组,一组一组的求工资sal的和

#查询的执行顺序
一: 语法顺序
    1. select
    	...
    2. from
    	...
    3. where
    	...
    4. group by
    	...
    5. having
    	...
    6. order by
    	...
     以上关键字只能按照这个顺序来, 不能颠倒
二: 执行顺序
    1. from       
    2. where
    3. group by  //分组
    4. having  //过滤
    5. select  
    6. order by  //排序
    	👇👆
   1> 从某张表中查询数据
   2> 先经过where条件筛选出有价值的数据
   3> 对这些有价值的数据进行分组
   4> 分组之后可以使用 having 继续过滤筛选
   5> select 查询出来
   6> 最后排序输出

#子查询
子查询可以出现在哪里:
    SELECT
    	...(SELECT)	
     FROM
    	...(SELECT)
     WHERE
     	...(SELECT)

#连接查询
SELECT c1.tid,c1.tname,c2.ntitle,c3.ccontent FROM 
	表1 c1 
 RIGHT JOIN 
    表2 c2 ON c1.tid=c2.ntid  
    #此时,c1表根据两者的关系字段,连接了c2表,可以获取他们两个表中的数据,
     (因为此处使用的是右连接,所以以c2表为主表展示数据,c1为从表,只匹配符合连接条件的数据,不符合的则不查询)
	#如果c1或c2还有其他关系字段可以关联到其他表,可以继续连接,然后使用关联字段继续连接更多的表,整合更多的数据,
 RIGHT JOIN 
    表3 c3 ON c3.cnid=c2.nid
    如果还有其他的表和这三个表中某个表有关联字段,则可以继续连接!

```

4. WHERE 与 HAVING 对比

   ```mysql
       >使用HAVING 可以对分完组之后的数据进行进一步过滤
   
       >HAVING不能单独使用,HAVING不能代替WHERE,
   
       >HAVING必须和GROUP BY 联合使用
   
       >优化策略: WHERE 和 HAVING , 优先选择 WHERE , WHERE实在完成不了,再选择HAVING
   ```

5. 连接

（1）内连接：驱动表中没有得到匹配的不会被加入

```mysql
select * from 驱动表, 被驱动表;
select * from 驱动表 join 被驱动表;
select * from 驱动表 inner join 被驱动表;
select * from 驱动表 cross join 被驱动表;
```

（2）外连接：驱动表中没有得到匹配的也会被加入，包括左右连接

```mysql
select * from 驱动表 left join 被驱动表 on 连接条件;
select * from 被驱动表 right join 驱动表 on 连接条件;
```

6. 连接中的过滤条件

**where**： 在外连接中不匹配的不会被加入。既不遵循内外连接定义（外连接也不能插入不匹配）。

**on**：在外连接中，不匹配的会被加入。既遵循内外连接定义。

# 二、MySQL的优化

1. 索引的存储类型

（1）B+树索引类型

在B+树上存储索引的节点，可以用于范围查找

（2）哈希索引

通过哈希函数进行映射，若产生冲突，就放入同一个桶里面，不能范围查找，但等值查找很快

（3）有序数组

可以等值查找、范围查找，但不适合频繁插入删除。

（4）二叉搜索树

对有序数组的优化，能log级别查找，也能实现快速插入删除





2. 表的优化

（1）尽量不使用INT数据类型，而使用更精确的INT类型

（2）尽量不使用字符串，使用INT代替

（3）尽量不使用datatime，使用TIMESTAMP代替

（4）只申请需要的空间，同（1）。

（5）单表不要有太多字段，可以多放几张表上，不要记录大量重复信息，又丑又慢

（6）尽量少使用NULL，很难查询优化并且占用额外的存储空间。第一个原因是，如果数据类型是可NULL的，会为每一行设置一个标志位，标志是不是NULL，占用空间且性能较差。第二个原因是，如果使用了NULL，当需要更新时，Mysql无法进行原地更新，从而导致索引分裂，影响性能。

3. 慢查询

当SQL执行的时间超过long_query_time参数设定的时间阈值（默认10s）时，就被认为是慢查询。慢查询被记录在慢查询日志里。

执行语句

开启慢查询：set global slow_query_log=on;

设置慢查询的阈值：set global long_query_time=1;

设置慢查询输出文件：set global slow_query_log_file=‘文件名’

查看属性：show variables like "%属性名"

查询用时最多的10条慢语句：mysqldumpslow -s t -t 10 -g 'select' slow_query_log_file

4. sql语句优化

查询语句应该尽量避免全表扫描：

（1） 首先应该考虑在Where子句以及OrderBy子句上建立索引。对于OrderBy可以使用索引的最左前缀。

（2）应尽量不使用 NULL值判断，因为NULL值判断会导致mysql放弃使用索引而进行全表扫描。可以使用0代替。

（3）尽量不使用!=和><判断符号，否则将会引起全表扫描。

（4）尽量不使用in、not in、or，否则会引起全表扫描

（5）like的首位应尽量避免%，否则会引起全表扫描

（6）应避免使用计算，用常量代替，否则将会引起全表扫描

（7）很多时候用 exists 代替 in 是一个好的选择：exists用于检查子查询是否至少会返回一行数据，该子查询实际上并不返回任何数据，而是返回值true或false。

select num from a where num in(select num from b)

用下面的语句替换：select num from a where exists (select 1 from b where num=a.num)

（8）尽量不要使用*，而用具体的字段代替。

（9）用where代替having子句
（10）在使用Union操作符时，应该考虑是否可以使用Union ALL来代替，因为Union操作符在进行结果合并时，会对产生的结果进行排序运算，删除重复记录，对于没有该需求的应用应使用Union ALL，后者仅仅只是将结果合并返回，能大幅度提高可以性能；
（11）Where子句中，表连接条件应该写在其他条件之前，因为Where子句的解析是从后向前的，所以尽量把能够过滤到多数记录的限制条件放在Where子句的末尾；
（12）若数据库表上存在诸如index(a,b,c)之类的联合索引，则Where子句中条件字段的出现顺序应该与索引字段的出现顺序一致，否则将无法使用该联合索引；
（13）From子句中表的出现顺序同样会对SQL语句的执行性能造成影响，From子句在解析时是从后向前的，即写在末尾的表将被优先处理，应该选择记录较少的表作为基表放在后面，同时如果出现3个及3个以上的表连接查询时，应该将交叉表作为基表；

优化诀窍：

**优化总结口诀**

全值匹配我最爱， 最左前缀要遵守；

带头大哥不能死， 中间兄弟不能断；

索引列上少计算， 范围之后全失效；

LIKE 百分写最右， 覆盖索引不写 *；

不等空值还有OR， 索引影响要注意；

VAR 引号不可丢， SQL 优化有诀窍。

5. Explain

Explain +sql语句，对sql语句进行分析，显示即将验证查询的顺序，返回的信息是执行信息可以用于验证是否使用了索引。执行信息包括| id | select_type | table | partitions | type | possible_keys | key | key_len | ref | rows | filtered | Extra |

id：键值

select_type：数据读取操作的类型，主要是用于区别普通查询、联合查询、子查询等的复杂查询

![image-20230303145430925](/Users/tom/Library/Application Support/typora-user-images/image-20230303145430925.png)

table：查询的表

partitions：

 type：访问的类型，比如是全表扫描还是索引

![image-20230303145622517](/Users/tom/Library/Application Support/typora-user-images/image-20230303145622517.png)

possible_keys：哪些索引可以被使用

key：哪些索引实际被应用了

key_len：索引消耗的字节数

ref：哪些列或者常量被用于了索引查询

rows：每张表有多少行被优化器查询

Extra：一些必要的信息，Using filesort 、Using temporary 、Using index 、Using where 、Using join buffer 、impossible where 、select tables optimized away 、distinct

6. 查询语句执行过程

（1）连接器：负责和客户建立连接，获取权限，维持和管理连接

（2）查询缓存：执行前先查询缓存

（3）分析器：词法分析和语法分析

（4）优化器：决定使用哪个索引，是否使用索引，怎么确定表的连接顺序

（5）执行器：执行前查询当前用户是否有权限，然后有权限则执行语句

7. 更新语句的执行过程

![](/Users/tom/Library/Application Support/typora-user-images/image-20230304161743358.png)

8. 子查询的优化方式

子查询可以出现在from,where等语句后面，优化方式有：

（1）子查询合并：将相同的子查询语句合并，这样就只需要查询一次就行，减少查询次数。

（2）子查询展开（子查询上拉）：将子查询语句变成多表连接操作，比如

SELECT * FROM t1,(SELECT * FROM t2 WHERE t2.a2>10) v_t2

WHERE t1.a1<10 AND v_t2.a2<20;

可优化为：

SELECT * FROM t1,t2 WHERE t1.a1<10 AND t2.a2<20 AND t2.a2>10;

优化前：执行子查询，执行连接，执行查询

优化后：执行连接，执行查询

（3）聚集子查询消除

Mysql不支持，因为Mysql认为对于聚集子查询只需要执行一次，执行完以后从缓存里面删除，因此没必要使用狙击子查询消除。另一方面，聚集子查询消除在索引列上执行，会更快得到查询结果，更能加速查询速度。、

9. MySQL不支持对如下情况的子查询进行优化

（1）带有UNION操作。

（2）带有GROUPBY、HAVING、聚集函数。

（3）使用ORDERBY中带有LIMIT。

（4）内表、外表的个数超过MySQL支持的最大表的连接数



# 三、锁

1. 锁的种类

（1）全局锁：把整个数据库加锁，只允许读，不允许写，命令是 Flush tables with read lock (FTWRL)

（2）表级锁：一种是表锁，一种是元数据锁。表锁：每次访问锁住整张表，开销小，并发度低。元数据锁：当正在访问时候加锁，有读锁写锁两种。

（3）行锁，每次锁住一行的数据，开销大，并发程度高。

在 InnoDB 事务中，⾏锁是在需要的时候才加上的，但并不是不需要了就⽴刻释放，⽽是要

等到事务结束时才释放。这个就是两阶段锁协议。 

对数据库来说分为

（1）共享锁：读锁

```mysql
#锁一行
SELECT user_id FROM product_comment WHERE user_id = 10 LOCK IN SHARE
MODE;
#锁一张表
LOCK TABLE product_comment READ;
#解锁
UNLOCK TABLE;
```



（2）排它锁：写锁

```mysql
#锁一行
SELECT user_id FROM product_comment WHERE user_id = 10 FOR UPDATE;
#锁一张表
LOCK TABLE product_comment WRITE;
#解锁
UNLOCK TABLE;
```



（3）意向锁：简单来说就是给更⼤⼀级别的空间⽰意⾥⾯是否已经上过锁。比如当表中某一行有排它锁，那么可以给表上意向锁，表明表有人在使用，无法再获取整张表。有意向共享锁，和意向排它锁



从程序员角度

（1）乐观锁：先不锁，出问题解决冲突，并发程度高

（2）悲观锁：提前锁

2. 加锁规则

两个原则： 

（1）加锁的基本单位是 next-key lock。希望你还记得，next-key lock 是前开后闭区 

间。 

（2）查找过程中访问到的对象才会加锁。

两个优化： 

（1） 索引上的等值查询，给唯⼀索引加锁的时候，next-key lock 退化为⾏锁。 

（2）索引上的等值查询，向右遍历时且最后⼀个值不满⾜等值条件的时候，next-key

lock 退化为间隙锁。 

⼀个**"bug"**：

（1） 唯⼀索引上的范围查询会访问到不满⾜条件的第⼀个值为⽌。（MySQL8.0之后的

版本已经修复）

# 四、事务

1. 什么是事务

（1）一个不可分割的最小单元

（2）一个事务对应一个完整的业务

2. 事务的特性

（1）原子性：不可分割的

（2）隔离性：互不影响的

（3）一致性：数据库完整性不能破坏

（4）持久性：数据永久保存的

3. 事务隔离级别

（1）RU 读不提交（Read Uncommited） 

会脏读

（2）RC读提交（Read Commited） 

会读不一致

（3）RR可重复读（Repeatable Read） 

读一致，不保证互斥

（4）serializable串⾏化 

保证不同事务之间的互斥 

mysql查询隔离级别的语句：show variables like "%transaction-isolation"

事务隔离的实现是通过回滚操作来实现的，回滚操作：记录最新状态之前的一个值。

4. 幂等操作

同一操作多次请求结果一致。

5. lvs负载均衡

IVS的IP负载均衡技术主要通过IPVS实现，IPVS虚拟⼀个IP地址，⽤户必须通过这个虚拟的IP地址访问服务器。该虚拟IP被称为LVS的VIP，访问的请求⾸先经过LVS的VIP，到达负载调度器，由负载调度器从real server列表中选取⼀个服务节点响应⽤户的请求。

简而言之，就是用户先通过虚拟ip找到负载调度器，负载调度器将任务分发给相对空闲的服务器。

6. Nginx负载均衡

工作在应用层，需要建立连接以后才进行转发，效率相对于lvs较低，但因为其工作在应用层，认为可操作性高，可以理解并根据某种特定的需求进行分发。

7. 常见的分布式的锁

（1）MySQL

（2）Redis

（3）ZK(Zookeeper)

8. 分布式中的一致性哈希算法

通过哈希函数将节点映射到哈希环上，再将<key,ip>信息映射到环上，并存储在顺时针遇到的第一个节点处，从而实现logn级别的服务器查找。

一致性哈希的要求

（1）平衡性

（2）单调性

（3）离散性

（4）负载均衡

9. CAP原理

（1）C（⼀致性）：对于分布在不同节点上的数据，如果⼀个节点更新数据之后，其他节点都能读到这个最新的数据

（2）A（可⽤性）：⾮故障节点在合理的时间内返回合理的响应 

（3）P（分区容错性）：出现⽹络错误，系统还能继续⼯作 

CAP原理说的是**CAP**三个条件不能同时共存：

CA：分布式中⼀般不会选择

CP：CP的代表是Zookeeper，放弃可⽤性，追求⼀致性和容错性

AP：追求分区容错性与可⽤性 

10. BASE理论

Basically Available（基本可⽤）：分布式系统在出现故障时，允许损失部分可⽤功能

Soft State（软状态）：允许系统中存在中间状态，这个状态不影响系统可⽤性

Eventually consistent（最终⼀致性）：在经过⼀段时间之后，所有节点的数据都会达到⼀

致 

BASE理论指的是一种评判标准，在强一致性无法满足的时候，可以退而求其次满足BASE理论，满足BASE理论要求的事务被称为柔事务。

11. InnoDB

InnoDB索引都是B+树结构的索引

在InnoDB里索引类型分为主键索引（聚簇索引）和非主键索引（二级索引）

主键索引，主键在B+树中，查询到主键以后，指向一行记录

非主键索引：先查询主键，再根据主键执行主键查询

12. InnoDB索引维护

能直接插入的直接插入，不能直接插入的产生分裂。

能直接删除的直接删除，不能直接删除的产生合并。

13. 覆盖索引

当执行如下语句时，这时只需要查ID的值，这时候ID的值已经在k索引树上了，因此可以直接返回。

```mysql
select ID from T where k between 3 and 5
```

14. 最左前缀原则

会将条件的最左和索引的第一个字段进行匹配。

15. 索引下推

MySQL5.6之前，查询主键以后就直接查询表，判断条件

MySQL5.6之后，查询到主键以后继续查询后面的条件，若不符合直接删除。

16. InnoDB的统计数据

（1）表的统计数据：show table status

（2）索引的统计数据：show index

统计的数据又分为永久性存储和非永久性存储两种

16. 如何避免长事务对业务的影响？

    从应⽤开发端来看**:**

    （1） 确定是否使⽤了set autocommit=0，如是，则改成1

    （2）确定是否有不必要的只读事务

    （3）业务连接数据库的时候，控制每个语句执⾏的最长时间 set maxexecutiontime 

    从数据库端来看**:**

    （1）监控相关表，设置长事务阈值，超过就报警/kill

    （2）可使⽤percona的pt-kill⼯具

    （3）在功能测试阶段输出所有的log，分析⽇志提前发现问题

    （4）把innodbundotablespaces设置成2或者更⼤的值

# 五、高性能

1. Mysql主从复制

从节点复制主节点的数据库或特定的表，默认方式是异步方式

2. 主从复制的用途

（1）数据备份

（2）读写分离

（3）架构扩展：负载均衡，降低主节点io压力

3. 主从复制原理

主节点线程：binary log dump thread：从节点连接主节点时候，主节点创建线程，用于发送bin-log内容

从节点线程： I/O thread、SQL thread：第一个用于接收主线程发送的数据，并保存到relay log中。第二个适用于读取relay log里面的内容，解析具体的操作并执行，保证主从数据的一致性。

基本过程：从节点io进程连接主节点并请求日志，主节点读取请求并发送日志，从节点接收请求并保存到relar log里面，从节点解析操作保证一致性。

4. 主从复制模式

（1）异步模式

（2）半同步模式

（3）全同步模式

（4）GTID复制模式

5. 分库分表

单库太大导致io瓶颈，单表太大导致索引膨胀，所以要进行拆分

拆分方案：水平拆分和垂直拆分

（1）水平拆表：根据规则将表切分，从而减少大量重复数据，比如学生表里面的班级信息可以拆分出来，变成两个表。

（2）垂直拆表：将大表变小表，分割列，4列变成两个表，一个表2列或3列，不常用的字段分离

（3）水平分库：将单张表的数据水平拆分，然后存储到其他的数据库

（4）垂直拆库：将表分类，业务相同或类似的放一个库里面。

⽔平分库分表切分规则：

range： 根据范围，⽐如0-1000⼀个表，1001到2000⼀个表

hash取模： ⽐如取ID，进⾏hash取模，根据模数分配到不同的数据库中地理区域： 按照地理范围进⾏划分 

时间范围： 按照时间进⾏切分

6. 高可用方案类型

（1）基于主从复制

（2）基于Galera协议

（3）基于NDB协议

（4）基于中间/proxy

（5）基于共享存储

（6）基于主机高可用


