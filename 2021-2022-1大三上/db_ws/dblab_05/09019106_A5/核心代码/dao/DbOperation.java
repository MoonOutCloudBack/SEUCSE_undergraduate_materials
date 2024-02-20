package dao;

import java.sql.*;

public class DbOperation {
    public static Connection con;
    public Statement statement;
    public static String driver = "com.mysql.cj.jdbc.Driver";
    public static String url = "jdbc:mysql://localhost:3306/exhi_management?" +
            "characterEncoding=utf-8&useSSL=false&serverTimezone=GMT&allowPublicKeyRetrieval=true";
    public static String user = "root";
    public static String password = "root"; // 这里用户名和密码，每个人根据自己电脑上的配置改动

    static { // 只做一次
        try {
            Class.forName(driver);
            con = DriverManager.getConnection(url, user, password);
            if (!con.isClosed()) {
//                System.out.println("数据库连接成功");
            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
            System.out.println("数据库驱动没有安装");
        } catch (SQLException e) {
            e.printStackTrace();
            System.out.println("数据库连接或操作失败");
        }
    }

    public DbOperation() {
        try {
            statement = con.createStatement();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public ResultSet search(String sql) { // 数据库查询操作，sql为数据库操作指令
        ResultSet resultSet = null;
        try {
            resultSet = this.statement.executeQuery(sql);
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return resultSet;
    }

    public void adm(String sql) { //adm为add delete modify 增删改的意思
        try {
            statement.executeUpdate(sql);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        DbOperation test = new DbOperation();
        DbOperation test2 = new DbOperation();
    }
}


