#Redis 速通

##启动！
>*redis-cli*

##string
####设置和获取
```
SET name chilkid //设置键“name”为"chilkid"
GET name //得到键”name“的值， 即chilkid
```

####删除
```
DEL name // 删除键“name”
```

####判断存在
```
EXISTS name // 判断以“name"为名字的键是否存在
```

####查找键
```
KEYS * //查找所有键
KEYS *me //查找所有满足以me结尾的键
```

####删库跑路
```
FLUSHALL //rm -rf(对的对的，兄弟对的)
```

####时限相关
```
TTL name  //查看name这个键的剩余时间， -1为没设置时限， -2为已经寄了
EXPIRE name 10 //设置name的过期时间是10秒
SETEX name 5 piasticOuo // 设置一个叫name的键， 过期时间是5秒， 值是塑料大神
SETNX name u2x1 //如果键name存在， 什么也不做， 否则创建
```

####注意事项
```
redis-cli --raw //以原始形式显示内容
```

##List

####入队
```
LPUSH letter a //往列表letter里头插一个元素'a'
RPUSH letter b //往列表letter里尾插一个元素'b'
LPUSH letter c d e //往列表头插'c','d','e'三个元素
```

####遍历
```
LRANGE letter 0 -1 //遍历列表letter内所有元素
```

####出队
```
RPOP letter //删除列表letter最后一个元素
LPOP letter //删除列表第一个元素
LPOP letter 2 //删除列表最后两个元素
LTRIM letter 1 3 //只保留列表元素1到元素3
```

####长度
```
LLEN letter //查看列表letter长度
```

##set

####添加
```
SADD course Redis //往集合course里添加成员Redis
```

####查询
```
SMEMBERS course //查看集合course内元素
SISMEMBER course Redis //查看Redis是否为course中的元素
```

####删除
```
SREM course Redis //删除集合course中元素Redis
```

####注意事项
```
如同std::set， 不会有重复的元素
SINTER SUNION SDIFF等命令， 计算交集并集
```

##SortedSet

####添加
```
ZADD result 3100 塑料 800 童 4000 游客 //向有序集合result中添加三个元素， 分数在前， 成员在后， 用空格隔开
```

####查询
```
ZRANGE result 0 -1 //查询有序集合result第一个元素到最后一个元素， 只输出成员， 按分数排序
ZRANGE result 0 -1 WITHSCORES //把分数也输出出来
ZSCORE result 童 //输出成员对应的分数
ZRANK result 童 //输出成员的排名（从小到大的index）
ZREVRANK result 童 //输出成员的排名（从大到小的index）
```

####删除
```
ZREM result 童 //有序集合result删除成员童
```

####注意事项
```
还有很多操作，如修改分数等
```

##Hash

####添加
```
HSET person name chilkid //往哈希person中添加键值对{name: chilkid}
HSET person age 100 //往哈希person中添加键值对{name: chilkid}
```

####查询
```
HGET person age //查询哈希person中age的映射
HGETALL person //获取整个哈希person中的键值对
HEXISTS person age //查询哈希person中age是否存在
HKEYS person //查询哈希person中所有键
HLEN person //查询哈希person中所有键值对的数量
```

####删除
```
HDEL person age //删除哈希person中的age
```

##subscribe

####接收
```
SUBSCRIBE chilkid //订阅频道chilkid， 接收chilkid频道的信息
```

####发送
```
PUBLISH chilkid redis // 发布一个叫redis的消息到频道chilkid
```

####注意事项
```
消息无法持久化
无法记录历史消息
```

##Stream（消息队列）

####添加
```
XADD chilkid * course redis // *表示自动生成一个消息的ID，添加消息内容是“课程是redis”
```

####查询
```
XLEN chilkid  //查看stream中消息数量
XRANGE chilkid - + //表示所有的消息
```

####删除
```
XDEL chilkid 1686495710450-0 //XDEL + stream名 + 消息ID， 删除消息
XTRIM chilkid MAXLEN 0 //表示删除所有消息
```

####消费
```
XREAD COUNT 2 BLOCK 1000 STREAMS chilkid 0 //读两条消息、 没消息时阻塞1000毫秒， 从chilkid的头开始读， 可以重复读取
XREAD COUNT 2 BLOCK 1000 STREAMS chilkid $ //获取从现在开始以后的最新消息
XGROUP CREATE chilkid group1 0 // 消息名称chilkid, 组名称group1, id0， 创建group1消费组
XINFO GROUPS chilkid //查看消费者组信息
XGROUP CREATECONSUMER chilkid group1 consumer1 //消息名字chilkid， 组名group1， 消费者名consumer1， 添加消费者
XREADGROUP GROUP group1 consumer1 COUNT 2 BLOCK 3000 chilkid >
//组名group1, 消费者名consumer1， count 2读取两条， BLOCK 3000阻塞3000毫秒， chilkid 消息名称， >表示从消息中读取最新的消息
```

##地理位置

####添加
```
GEOADD city 116.405285 39.904989 beijing //地理位置信息名字是city， 经度， 纬度， 城市名字， 也可以同时添加多个
```

####查询
```
GEOPOS city beijing //查询城市经纬度
GEODIST city beijing shanghai //查询城市间距离
GEOSEARCH city FROMMEMBER shanghai BYRADIUS 300 // 搜索指定范围内成员， 以成员位置或指定的经纬度为中心，BYRADIUS表示圆形范围， 300千米半径， BYBOX表示矩形范围
```

##HyperLogLog

####
```
原理使用随机算法，牺牲精确度，换取更小内存消耗
统计网站uv， 词的搜索次数等等
```

####添加
```
PFADD course git docker redis  //添加元素course
PFMERGE result course course2  //合并course和course2，结果放在result里
```

####查询
```
PFCOUNT course //查看基数
```

##位图

####设置
```
SETBIT ii 0 1 //将键ii在偏移量为零的位置上设置为1
SETBIT ii 1 0 //将键ii在偏移量为一的位置上设置为0
SET ii "\xF0" //十六进制， 设置为11110000
```

####查询
```
GETBIT ii 0 //获取某个偏移量的值
BITCOUNT ii //统计key的值有多少bit是1
BITPOS ii 0 //第一个出现0的位置
BITPOS ii 0 [start, end] //指定从start到end
```

##位域

####设置
```
BITFIELD player:1 set u8 #0 1 // key表示id, u8类型整数表示等级， #0表示第一个位置
BITFIELD player:1 set u32 #1 100 // key表示id, u32类型整数表示金钱， #1表示第二个位置
```

####查询
```
BITFIELD player:1 get u8 #0 // 查询第一个位置的值， 即等级
```

####修改
```
BITFIELD player:1 incrby u32 #1 100 // 金钱增加100
```

##事务

####概述
```
MULTI 命令用于开启事务， 事务开启后所有命令被放入一个队列， 最后通过EXEC命令执行事务中所有命令
Redis事务和其他关系型数据库的事务不太一样， 关系数据库事务一般是一个原子操作， 要么全部成功， 要么全部失败
Redis保证三点：
EXEC前所有命令会被缓存起来不会立即执行
收到EXEC后命令开始执行，事务中任何一个命令执行失败， 其他命令仍然执行
事务执行过程中，其他客户端提交命令请求并不会被插入到事务的执行命令序列中
```

##持久化

####RDB方式
```
指定时间间隔内将内存数据快照写入磁盘
通过配置文件中的save参数配置, 参数<seconds> <changes>

save命令手工触发快照
bgsave单独创建子进程负责将内存数据写入硬盘，主进程继续处理请求， 会有一定性能消耗
```

####AOF方式
```
不仅将命令写入内存，还会将命令写入到一个追加文件中
该文件就是AOF文件，以日志形式记录每一个写操作
重新执行AOF中命令，重建数据
配置文件appendonly no -> appendonly yes
```

##主从复制

####概述
```
是把redis服务器数据复制到其他redis服务器
主节点能有多从节点，从节点只能有一个主节点，数据单向(主->从)
主写从读
```

####命令行配置从节点
```
slaveof直接指定主节点IP和端口, 不常用
```

####配置文件
```
修改redis.conf
修改port //端口
修改pidfile //pid信息
修改dbfilename //持久化文件
replicaof <masterip> <masterport>
```

##哨兵模式

####概述
```
实现故障转移
哨兵以一个独立进程运行在redis集群中，监控redis集群各个节点是否运行正常
如果发现某个节点出了问题，哨兵就会通过发布订阅模式来通知其他节点
自动故障转移，将一个从节点升级为主节点，然后将其他从节点指向新主节点
```

####如何配置
```
redis -sentinel启动哨兵节点
添加配置文件sentinel.conf
sentinel monitor [监控主节点的名称] [主节点IP] [主节点端口号] 1 //只需要一个哨兵节点就可以进行故障转移
redis -sentinel +配置文件名字
```

####注意
```
哨兵也是进程，也可能错
实际可能会用三个哨兵(选举领导者)
```

##性能管理

####内存碎片率

```
Redis 需要分配连续内存块来存储 1G 的数据集。如果物理内存上没有超过 1G 的连续内存块， 那操作系统就不得不使用多个不连续的小内存块来分配并存储这 1G 数据，该操作就会导致内存碎片的产生.
跟踪内存碎片率对理解Redis实例的资源性能是非常重要的：
●内存碎片率稍大于1是合理的，这个值表示内存碎片率比较低，也说明 Redis 没有发生内存交换。
●内存碎片率超过1.5，说明Redis消耗了实际需要物理内存的150%，其中50%是内存碎片率。需要在redis-cli工具上输入shutdown save 命令，让 Redis 数据库执行保存操作并关闭 Redis 服务，再重启服务器。
●内存碎片率低于1的，说明Redis内存分配超出了物理内存，操作系统正在进行内存交换。需要增加可用物理内存或减少 Redis 内存占用。
```

####内存使用率
```
redis实例的内存使用率超过可用最大内存，操作系统将开始进行内存与swap空间交换。
#避免内存交换发生的方法：
●针对缓存数据大小选择安装 Redis 实例
●尽可能的使用Hash数据结构存储
●设置key的过期时间
```

####内回收key
```
当达到设置的最大阀值时，需选择一种key的回收策略，默认情况下回收策略是禁止删除。
配置文件中修改 maxmemory-policy 属性值：
●volatile-lru：使用LRU算法从已设置过期时间的数据集合中淘汰数据(移除最近最少使用的key，针对设置了TTL的key)
●volatile-ttl：从已设置过期时间的数据集合中挑选即将过期的数据淘汰（移除最近过期的key）
●volatile-random：从已设置过期时间的数据集合中随机挑选数据淘汰（在设置了TTL的key里随机移除）
●allkeys-lru：使用LRU算法从所有数据集合中淘汰数据（移除最少使用的key，针对所有的key）
●allkeys-random：从数据集合中任意选择数据淘汰（随机移除key）
●noenviction：禁止淘汰数据（不删除直到写满时报错）
```
