

# XAnalysis软件分析系统详细设计描述文档

## 文档修改历史

| 修改人员 |   日期    | 修改原因 |   版本号   |
| :------: | :-------: | :------: | :--------: |
|  李甘霖  | 2020.6.21 | 最初草稿 | v1.0.0草稿 |
|  李俊杰  | 2020.6.22 |          |   v1.0.1   |



[TOC]

# 引言

## 编制目的

本报告详细完成对博客系统的详细设计，达到指导后续软件构造的目的，同时实现和测试人员及用户的沟通。
本报告面向开发人员、测试人员及最终用户而编写，是了解系统的导航。

## 结构视角

### 用户界面的分解

根据需求，系统存在如下的用户界面：登陆界面、注册用户界面、用户主界面、个人基本信息维护界面、修改密码界面、上传文件界面、查看信息界面、历史记录界面。

#### 展示层模块的职责

展示层模块的职责如下表所示。

| 模块     | 职责                                                     |
| -------- | -------------------------------------------------------- |
| loginui  | 登录界面包，负责允许用户登录或注册账户，进入主界面的功能 |
| modifyui | 修改信息界面                                             |
| uploadui | 上传文件界面                                             |
| recordui | 历史记录界面                                             |
| detailui | 分析信息界面                                             |

##### 展示层需要的接口规范

| 提供的服务（供接口）               | 职责             |
| ---------------------------------- | ---------------- |
| AccountServiceImpl.login           | 登录             |
| AccountServiceImpl.registerAccount | 注册             |
| AccountServiceImpl.getUserInfo     | 获得用户信息     |
| AccountServiceImpl.updateUserInfo  | 更新用户信息     |
| AccountServiceImpl.change-password | 修改密码         |
| AccountServiceImpl.reset-password  | 重置             |
| FileServiceImpl.UploadSingle       | 单篇代码上传实现 |
| FileServiceImpl.detail             | 查看分析结果     |
| FileServiceImpl.getRecord          | 获取历史记录     |





### 业务逻辑层的分解

根据体系结构的设计，我们使用分层的模式来构建本系统，将系统分为web展示层、restful api层、业务逻辑层、数据层。每一层之间为了增加灵活性，我们会添加接口。restful api层和业务逻辑层之间添加FileServiceImpl等接口。业务逻辑层和数据层之间添加FileRepository等接口。这样，我们将依赖于接口而非实体类，增加了系统的灵活性。

#### Filebl模块

##### 模块内部类的接口规范

###### FileServiceImpl

| 提供的服务（供接口）          | 职责                         |
| ----------------------------- | ---------------------------- |
| FileServiceImpl.UploadSingle  | 单篇代码上传实现             |
| FileServiceImpl.getRecordList | 根据用户ID获得用户检测记录   |
| FileServiceImpl.getDetail     | 根据recordID获得用户检测详情 |
| FileServiceImpl.jna           | 通过jna调用dll               |

#### userbl模块

##### 模块内部类的接口规范

###### AccountServiceImpl

| 提供的服务（供接口）               | 职责         |
| ---------------------------------- | ------------ |
| AccountServiceImpl.login           | 登录         |
| AccountServiceImpl.registerAccount | 注册         |
| AccountServiceImpl.getUserInfo     | 获得用户信息 |
| AccountServiceImpl.updateUserInfo  | 更新用户信息 |
| AccountServiceImpl.change-password | 修改密码     |
| AccountServiceImpl.reset-password  | 重置         |



### 数据层分解

#### File模块

##### 模块内部类的接口规范

###### FileServiceImpl

| 提供的服务（供接口）                | 职责                   |
| ----------------------------------- | ---------------------- |
| FileRepository.findRecordsByUserId  | 根据用户id获得检测记录 |
| FileRepository.findRecordByFilename | 根据文件名获得检测记录 |
| FileRepository.saveRecord           | 存储一条记录           |
|                                     |                        |

#### user模块

##### 模块内部类的接口规范

###### AccountServiceImpl

| 提供的服务（供接口）              | 职责               |
| --------------------------------- | ------------------ |
| AccountRepository.login           | 登录               |
| AccountRepository.registerAccount | 注册               |
| AccountRepository.getUserInfo     | 根据id获得用户信息 |
| AccountRepository.updateUserInfo  | 根据id更新用户信息 |
| AccountRepository.reset           | 重置密码           |

### 信息视角

#### 数据持久化对象

在此对系统的实体类做简单的介绍：

- User：用户id、邮箱、密码、用户名、权限

  ```java
  private Long id;
  
      @NotBlank
      @Pattern(regexp = Constants.LOGIN_REGEX)
      @Size(min = 1, max = 50)
      private String login;
  
      @Size(max = 50)
      private String firstName;
  
      @Size(max = 50)
      private String lastName;
  
      @Email
      @Size(min = 5, max = 254)
      private String email;
  
      @Size(max = 256)
      private String imageUrl;
  
      private boolean activated = false;
  
      @Size(min = 2, max = 10)
      private String langKey;
  
      private String createdBy;
  
      private Instant createdDate;
  
      private String lastModifiedBy;
  
      private Instant lastModifiedDate;
  
      private Set<String> authorities;
  ```

- Record:id、时间、文件名、内容、分析结果

  ```java
  public class Record {
      @Id
      @GeneratedValue(strategy = GenerationType.IDENTITY)
      private int id;
  
      @Column(name="user_id",nullable = true)
      private int userId;
  
      @Column(name="warning",nullable = true)
      private String warning;
  
      @Column(name="filename",nullable = true)
      private String filename;
  
      @Column(name="content",nullable = true)
      private String content;
  
      @Column(name="time",nullable =true)
      private String time;
  }
  ```

  

