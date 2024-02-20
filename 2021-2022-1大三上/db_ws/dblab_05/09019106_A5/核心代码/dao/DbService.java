package dao;

import entify.*;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLClientInfoException;
import java.sql.SQLException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;

public class DbService {

    /* ====== 牟倪的函数 ====== */
    // 查询所有特展，id+name+起始日期
    public static List<Exhi_info3> getAllExhibition() throws SQLException {
        DbOperation db = new DbOperation();
        List<Exhi_info3> lst = new ArrayList<>();
        ResultSet res = db.search("SELECT E.eid, E.ename, E.start_date, E.end_date from exhibition E");
        String eid = null, ename = null, start_date = null, end_date = null;
        while (res.next()) {
            eid = res.getString("eid");
            ename = res.getString("ename");
            start_date = res.getString("start_date");
            end_date = res.getString("end_date");
            Exhi_info3 temp = new Exhi_info3();
            temp.setEid(eid);
            temp.setEname(ename);
            temp.setStart_date(start_date);
            temp.setEnd_date(end_date);
            lst.add(temp);
        }
        return lst;
    }

    // 用名称查询特展，id+name+起始日期，模糊搜索
    public static List<Exhi_info3> getExhibitionByName(String ename) throws SQLException {
        DbOperation db = new DbOperation();
        List<Exhi_info3> lst = new ArrayList<>();
        // 我们来模糊搜索吧！
        String tempname = "%";
        for (int i = 0; i < ename.length(); ++i) {
            tempname = tempname + ename.charAt(i) + "%";
        }
        ResultSet res = db.search("SELECT E.eid, E.ename, E.start_date, E.end_date from exhibition E" +
                " WHERE E.ename LIKE '" + tempname + "'");
        String eid = null, ename1 = null, start_date = null, end_date = null;
        while (res.next()) {
            eid = res.getString("eid");
            ename1 = res.getString("ename");
            start_date = res.getString("start_date");
            end_date = res.getString("end_date");
            Exhi_info3 temp = new Exhi_info3();
            temp.setEid(eid);
            temp.setEname(ename1);
            temp.setStart_date(start_date);
            temp.setEnd_date(end_date);
            lst.add(temp);
        }
        return lst;
    }

    // 用日期查询特展，id+name+起始日期
    public static List<Exhi_info3> getExhibitionByDate(String date) throws SQLException {
        DbOperation db = new DbOperation();
        List<Exhi_info3> lst = new ArrayList<>();
        ResultSet res = db.search("SELECT E.eid, E.ename, E.start_date, E.end_date from exhibition E " +
                "WHERE '" + date + "'BETWEEN E.start_date AND E.end_date");
        String eid = null, ename = null, start_date = null, end_date = null;
        while (res.next()) {
            eid = res.getString("eid");
            ename = res.getString("ename");
            start_date = res.getString("start_date");
            end_date = res.getString("end_date");
            Exhi_info3 temp = new Exhi_info3();
            temp.setEid(eid);
            temp.setEname(ename);
            temp.setStart_date(start_date);
            temp.setEnd_date(end_date);
            lst.add(temp);
        }
        return lst;
    }

    // 用城市查询特展，id+name+起始日期
    public static List<Exhi_info3> getExhibitionByCity(String city) throws SQLException {
        DbOperation db = new DbOperation();
        List<Exhi_info3> lst = new ArrayList<>();
        String tempname = "%";
        for (int i = 0; i < city.length(); ++i) {
            tempname = tempname + city.charAt(i) + "%";
        }
        ResultSet res = db.search("SELECT E.eid, E.ename, E.start_date, E.end_date from exhibition E" +
                " WHERE E.mid IN (SELECT M.mid FROM museum M WHERE M.city LIKE '" + tempname + "')");
        String eid = null, ename = null, start_date = null, end_date = null;
        while (res.next()) {
            eid = res.getString("eid");
            ename = res.getString("ename");
            start_date = res.getString("start_date");
            end_date = res.getString("end_date");
            Exhi_info3 temp = new Exhi_info3();
            temp.setEid(eid);
            temp.setEname(ename);
            temp.setStart_date(start_date);
            temp.setEnd_date(end_date);
            lst.add(temp);
        }
        return lst;
    }

    // 用文物名称查询特展，id+name+起始日期
    public static List<Exhi_info3> getExhibitionByTreasureName(String tname) throws SQLException {
        DbOperation db = new DbOperation();
        List<Exhi_info3> lst = new ArrayList<>();
        String tempname = "%";
        for (int i = 0; i < tname.length(); ++i) {
            tempname = tempname + tname.charAt(i) + "%";
        }
        ResultSet res = db.search("SELECT E.eid, E.ename, E.start_date, E.end_date FROM exhibition E" +
                " WHERE E.eid IN (SELECT S.eid FROM show S WHERE S.tid IN " +
                "(SELECT T.tid FROM treasure T WHERE T.tname LIKE '" + tempname + "'))");
        String eid = null, ename = null, start_date = null, end_date = null;
        while (res.next()) {
            eid = res.getString("eid");
            ename = res.getString("ename");
            start_date = res.getString("start_date");
            end_date = res.getString("end_date");
            Exhi_info3 temp = new Exhi_info3();
            temp.setEid(eid);
            temp.setEname(ename);
            temp.setStart_date(start_date);
            temp.setEnd_date(end_date);
            lst.add(temp);
        }
        return lst;
    }

    // 用博物馆名称查询特展，id+name+起始日期
    public static List<Exhi_info3> getExhibitionByMuseumName(String mname) throws SQLException {
        DbOperation db = new DbOperation();
        List<Exhi_info3> lst = new ArrayList<>();
        String tempname = "%";
        for (int i = 0; i < mname.length(); ++i) {
            tempname = tempname + mname.charAt(i) + "%";
        }
        ResultSet res = db.search("SELECT E.eid, E.ename, E.start_date, E.end_date FROM exhibition E" +
                " WHERE E.mid IN (SELECT M.mid FROM museum M WHERE M.mname LIKE '" + tempname + "')");
        String eid = null, ename = null, start_date = null, end_date = null;
        while (res.next()) {
            eid = res.getString("eid");
            ename = res.getString("ename");
            start_date = res.getString("start_date");
            end_date = res.getString("end_date");
            Exhi_info3 temp = new Exhi_info3();
            temp.setEid(eid);
            temp.setEname(ename);
            temp.setStart_date(start_date);
            temp.setEnd_date(end_date);
            lst.add(temp);
        }
        return lst;
    }

    // 特展id -> 博物馆信息，id+name
    public static List<Mu_info1> getMuseumByExhibitionId(String eid) throws SQLException {
        DbOperation db = new DbOperation();
        List<Mu_info1> lst = new ArrayList<>();
        ResultSet res = db.search("SELECT M.mid, M.mname FROM museum M" +
                " WHERE M.mid IN (SELECT M.mid FROM exhibition E WHERE E.eid = " + eid + ")");
        String mid = null, mname = null;
        while (res.next()) {
            mid = res.getString("mid");
            mname = res.getString("mname");
            Mu_info1 temp = new Mu_info1();
            temp.setMid(mid);
            temp.setMname(mname);
            lst.add(temp);
        }
        return lst;
    }

    // 特展id -> 文物信息，id+name
    public static List<Trea_info> getTreasureByExhibitionId(String eid) throws SQLException {
        DbOperation db = new DbOperation();
        List<Trea_info> lst = new ArrayList<>();
        ResultSet res = db.search("SELECT T.tid, T.tname, T.mid FROM treasure T" +
                " WHERE T.tid IN (SELECT S.tid FROM showlist S WHERE S.eid = " + eid + ")");
        String tid = null, tname = null, mid = null;
        while (res.next()) {
            tid = res.getString("tid");
            tname = res.getString("tname");
            mid = res.getString("mid");
            Trea_info temp = new Trea_info();
            temp.setTid(tid);
            temp.setTname(tname);
            temp.setMid(mid);
            lst.add(temp);
        }
        return lst;
    }

    public static void main(String[] args) {
        try {
            System.out.println("搜索所有特展：");
            List<Exhi_info3> tmp = DbService.getAllExhibition();
            for (Exhi_info3 info : tmp) {
                System.out.println("eid: " + info.getEid()
                        + ", ename: " + info.getEname()
                        + ", start_date: " + info.getStart_date()
                        + " , end_date: " + info.getEnd_date());
            }
            System.out.println("按特展名称“国家宝藏”搜索特展：");
            tmp = DbService.getExhibitionByName("国家宝藏");
            for (Exhi_info3 info : tmp) {
                System.out.println("eid: " + info.getEid()
                        + ", ename: " + info.getEname()
                        + ", start_date: " + info.getStart_date()
                        + " , end_date: " + info.getEnd_date());
            }
            System.out.println("按日期“2021-12-13”搜索特展：");
            tmp = DbService.getExhibitionByDate("2021-12-13");
            for (Exhi_info3 info : tmp) {
                System.out.println("eid: " + info.getEid()
                        + ", ename: " + info.getEname()
                        + ", start_date: " + info.getStart_date()
                        + " , end_date: " + info.getEnd_date());
            }
            System.out.println("按城市“南京”搜索特展：");
            tmp = DbService.getExhibitionByCity("南京");
            for (Exhi_info3 info : tmp) {
                System.out.println("eid: " + info.getEid()
                        + ", ename: " + info.getEname()
                        + ", start_date: " + info.getStart_date()
                        + " , end_date: " + info.getEnd_date());
            }
            System.out.println("按文物名称“面具”搜索特展：");
            tmp = DbService.getExhibitionByName("面具");
            for (Exhi_info3 info : tmp) {
                System.out.println("eid: " + info.getEid()
                        + ", ename: " + info.getEname()
                        + ", start_date: " + info.getStart_date()
                        + " , end_date: " + info.getEnd_date());
            }
            System.out.println("按博物馆名称“博物馆”搜索特展：");
            tmp = DbService.getExhibitionByName("博物馆");
            for (Exhi_info3 info : tmp) {
                System.out.println("eid: " + info.getEid()
                        + ", ename: " + info.getEname()
                        + ", start_date: " + info.getStart_date()
                        + " , end_date: " + info.getEnd_date());
            }
            System.out.println("用 特展id=1 搜索博物馆：");
            List<Mu_info1> tmp2 = DbService.getMuseumByExhibitionId("1");
            for (Mu_info1 info : tmp2) {
                System.out.println("mid: " + info.getMid()
                        + ", mname: " + info.getMname());
            }
            System.out.println("用 特展id=1 搜索文物：");
            List<Trea_info> tmp3 = DbService.getTreasureByExhibitionId("1");
            for (Trea_info info : tmp3) {
                System.out.println("tid: " + info.getTid()
                        + ", tname: " + info.getTname()
                        + ", mid: " + info.getMid());
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }


    }
}
