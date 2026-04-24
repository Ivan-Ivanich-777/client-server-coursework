#include "mainwindow.h"
#include <QApplication>
#include <QMap>

QString g_lang = "en";
QMap<QString, QString> tr_en, tr_ru;

void initTranslations() {
    tr_en["appName"]="Math Solver"; tr_ru["appName"]="Графики";
    tr_en["function"]="Function f(x)"; tr_ru["function"]="Функция f(x)";
    tr_en["login"]="Login"; tr_ru["login"]="Логин";
    tr_en["password"]="Password"; tr_ru["password"]="Пароль";
    tr_en["signIn"]="Sign In"; tr_ru["signIn"]="Войти";
    tr_en["createAccount"]="Create Account"; tr_ru["createAccount"]="Создать аккаунт";
    tr_en["wrongPass"]="Wrong login or password"; tr_ru["wrongPass"]="Неверный логин или пароль";
    tr_en["fillAll"]="Fill all fields!"; tr_ru["fillAll"]="Заполните все поля!";
    tr_en["regTitle"]="Registration"; tr_ru["regTitle"]="Регистрация";
    tr_en["confirmPass"]="Confirm password"; tr_ru["confirmPass"]="Подтвердите пароль";
    tr_en["email"]="Email"; tr_ru["email"]="Эл. почта";
    tr_en["register"]="Register"; tr_ru["register"]="Зарегистрироваться";
    tr_en["back"]="Back"; tr_ru["back"]="← Назад";
    tr_en["passMismatch"]="Passwords do not match!"; tr_ru["passMismatch"]="Пароли не совпадают!";
    tr_en["regSuccess"]="Registration successful!"; tr_ru["regSuccess"]="Регистрация успешна!";
    tr_en["userExists"]="User already exists!"; tr_ru["userExists"]="Пользователь уже существует!";
    tr_en["colors"]="🎨 Colors"; tr_ru["colors"]="🎨 Цвета";
    tr_en["graph1"]="Graph 1"; tr_ru["graph1"]="График 1";
    tr_en["graph2"]="Graph 2"; tr_ru["graph2"]="График 2";
    tr_en["graph3"]="Graph 3"; tr_ru["graph3"]="График 3";
    tr_en["graphBg"]="Graph Background"; tr_ru["graphBg"]="Фон графика";
    tr_en["sliderColor"]="Slider Color"; tr_ru["sliderColor"]="Цвет ползунка";
    tr_en["gridColor"]="Grid Color"; tr_ru["gridColor"]="Цвет сетки";
    tr_en["axisColor"]="Axis Color"; tr_ru["axisColor"]="Цвет осей";
    tr_en["resetAll"]="Reset All"; tr_ru["resetAll"]="Сбросить всё";
    tr_en["language"]="🌐 Language"; tr_ru["language"]="🌐 Язык";
    tr_en["english"]="English"; tr_ru["english"]="Английский";
    tr_en["russian"]="Русский"; tr_ru["russian"]="Русский";
    tr_en["theme"]="🌓 Theme"; tr_ru["theme"]="🌓 Тема";
    tr_en["darkTheme"]="Dark"; tr_ru["darkTheme"]="Тёмная";
    tr_en["lightTheme"]="Light"; tr_ru["lightTheme"]="Светлая";
    tr_en["logout"]="Logout"; tr_ru["logout"]="Выйти";
    tr_en["sliderX"]="X:"; tr_ru["sliderX"]="X:";
    tr_en["sliderA"]="A:"; tr_ru["sliderA"]="A:";
    tr_en["xAxis"]="X"; tr_ru["xAxis"]="X";
    tr_en["yAxis"]="f(x)"; tr_ru["yAxis"]="f(x)";
}
QString t(const QString& k) { if(g_lang=="ru"&&tr_ru.contains(k))return tr_ru[k]; return tr_en.value(k,k); }

QString darkStyle() { return R"(
    * { font-family: "Segoe UI", "Arial", sans-serif; font-size: 13px; }
    QMainWindow, QWidget { background-color: #0d1117; color: #c9d1d9; }
    QPushButton { background-color: #238636; border: 1px solid #2ea043; border-radius: 8px; padding: 10px 20px; color: white; font-weight: bold; }
    QPushButton:hover { background-color: #2ea043; }
    QPushButton#btnBack, QPushButton#btnToReg, QPushButton#btnLang { background-color: #21262d; border-color: #30363d; color: #c9d1d9; }
    QPushButton#btnLogout { background-color: #da3633; border-color: #f85149; color: white; }
    QLineEdit { background-color: #0d1117; border: 1px solid #30363d; border-radius: 8px; padding: 12px; color: #c9d1d9; font-size: 14px; }
    QLineEdit:focus { border-color: #58a6ff; }
    QSlider::groove:horizontal { background: #21262d; height: 8px; border-radius: 4px; }
    QSlider::handle:horizontal { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #79c0ff, stop:1 #58a6ff); width: 24px; height: 24px; margin: -8px 0; border-radius: 12px; border: 2px solid #c9d1d9; }
    QSlider::sub-page:horizontal { background: #58a6ff; border-radius: 4px; }
    QDoubleSpinBox { background-color: #0d1117; border: 1px solid #30363d; border-radius: 6px; padding: 6px; color: #c9d1d9; font-size: 13px; }
    QToolButton { background-color: #21262d; border: 1px solid #30363d; border-radius: 8px; color: #c9d1d9; font-size: 22px; font-weight: bold; padding: 6px 10px; }
    QToolButton:hover { background-color: #30363d; border-color: #58a6ff; }
    QMenu { background-color: #161b22; color: #c9d1d9; border: 1px solid #30363d; padding: 6px; }
    QMenu::item { padding: 8px 28px 8px 12px; }
    QMenu::item:selected { background-color: #21262d; border-radius: 4px; }
    QLabel { color: #c9d1d9; }
    QFrame#sidePanel { background-color: #161b22; border-right: 1px solid #30363d; }
    QFrame#topBar { background-color: #161b22; border-bottom: 1px solid #30363d; }
)"; }

QString lightStyle() { return R"(
    * { font-family: "Segoe UI", "Arial", sans-serif; font-size: 13px; }
    QMainWindow, QWidget { background-color: #ffffff; color: #24292f; }
    QPushButton { background-color: #0969da; border: 1px solid #0969da; border-radius: 8px; padding: 10px 20px; color: white; font-weight: bold; }
    QPushButton:hover { background-color: #0550ae; }
    QPushButton#btnBack, QPushButton#btnToReg, QPushButton#btnLang { background-color: #f6f8fa; border-color: #d0d7de; color: #24292f; }
    QPushButton#btnLogout { background-color: #cf222e; border-color: #cf222e; color: white; }
    QLineEdit { background-color: #ffffff; border: 1px solid #d0d7de; border-radius: 8px; padding: 12px; color: #24292f; font-size: 14px; }
    QLineEdit:focus { border-color: #0969da; }
    QSlider::groove:horizontal { background: #eaeef2; height: 8px; border-radius: 4px; }
    QSlider::handle:horizontal { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #79c0ff, stop:1 #0969da); width: 24px; height: 24px; margin: -8px 0; border-radius: 12px; border: 2px solid #ffffff; }
    QSlider::sub-page:horizontal { background: #0969da; border-radius: 4px; }
    QDoubleSpinBox { background-color: #ffffff; border: 1px solid #d0d7de; border-radius: 6px; padding: 6px; color: #24292f; font-size: 13px; }
    QToolButton { background-color: #f6f8fa; border: 1px solid #d0d7de; border-radius: 8px; color: #24292f; font-size: 22px; font-weight: bold; padding: 6px 10px; }
    QToolButton:hover { background-color: #eaeef2; border-color: #0969da; }
    QMenu { background-color: #ffffff; color: #24292f; border: 1px solid #d0d7de; padding: 6px; }
    QMenu::item { padding: 8px 28px 8px 12px; }
    QMenu::item:selected { background-color: #eaeef2; border-radius: 4px; }
    QLabel { color: #24292f; }
    QFrame#sidePanel { background-color: #f6f8fa; border-right: 1px solid #d0d7de; }
    QFrame#topBar { background-color: #f6f8fa; border-bottom: 1px solid #d0d7de; }
)"; }

bool g_dark = true;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    initTranslations();
    qApp->setStyleSheet(darkStyle());
    MainWindow w;
    w.setWindowTitle("Math Solver");
    w.showMaximized();
    return a.exec();
}
