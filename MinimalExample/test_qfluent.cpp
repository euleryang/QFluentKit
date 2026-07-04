#include <QObject>
#include <QTest>
#include <QString>
#include <QColor>
#include <QMetaObject>

#include <QFluent/Theme.h>
#include <QFluent/FluentIcon.h>

class TestQFluent : public QObject
{
    Q_OBJECT

private slots:
    // Theme tests
    void testThemeModeEnum();
    void testThemeModeEnum_data();
    void testToggleTheme();
    void testSetThemeMode();

    // FluentIcon / IconType tests
    void testIconTypeEnum();
    void testIconTypeEnum_data();
    void testFluentIconUtils_iconColor();

    // StyleSheet / Color tests
    void testColorParsing();
    void testColorParsing_data();
};

// ---- Theme tests ----

void TestQFluent::testThemeModeEnum()
{
    QFETCH(Fluent::ThemeMode, mode);
    QFETCH(QString, modeStr);

    // Verify enum value can be converted to string via QMetaEnum
    QMetaEnum metaEnum = QMetaEnum::fromType<Fluent::ThemeMode>();
    QString converted = metaEnum.valueToKey(static_cast<int>(mode));
    QCOMPARE(converted, modeStr);
}

void TestQFluent::testThemeModeEnum_data()
{
    QTest::addColumn<Fluent::ThemeMode>("mode");
    QTest::addColumn<QString>("modeStr");

    QTest::newRow("AUTO")  << Fluent::ThemeMode::AUTO  << "AUTO";
    QTest::newRow("LIGHT") << Fluent::ThemeMode::LIGHT << "LIGHT";
    QTest::newRow("DARK")  << Fluent::ThemeMode::DARK  << "DARK";
}

void TestQFluent::testToggleTheme()
{
    qDebug() << "=== testToggleTheme ===";
    // Save current theme
    Fluent::ThemeMode original = Theme::themeMode();

    // Toggle to DARK
    Theme::setThemeMode(Fluent::ThemeMode::LIGHT);
    Theme::toggleTheme();
    QCOMPARE(Theme::themeMode(), Fluent::ThemeMode::DARK);
    qDebug() << "  LIGHT -> toggle -> DARK: OK";

    // Toggle back to LIGHT
    Theme::toggleTheme();
    QCOMPARE(Theme::themeMode(), Fluent::ThemeMode::LIGHT);
    qDebug() << "  DARK -> toggle -> LIGHT: OK";

    // Restore
    Theme::setThemeMode(original);
    qDebug() << "  PASSED";
}

void TestQFluent::testSetThemeMode()
{
    Theme::setThemeMode(Fluent::ThemeMode::DARK);
    QCOMPARE(Theme::themeMode(), Fluent::ThemeMode::DARK);

    Theme::setThemeMode(Fluent::ThemeMode::LIGHT);
    QCOMPARE(Theme::themeMode(), Fluent::ThemeMode::LIGHT);

    Theme::setThemeMode(Fluent::ThemeMode::AUTO);
    QCOMPARE(Theme::themeMode(), Fluent::ThemeMode::AUTO);
}

// ---- IconType tests ----

void TestQFluent::testIconTypeEnum()
{
    QFETCH(Fluent::IconType, icon);
    QFETCH(QString, iconStr);

    QMetaEnum metaEnum = QMetaEnum::fromType<Fluent::IconType>();
    QString converted = metaEnum.valueToKey(static_cast<int>(icon));
    QCOMPARE(converted, iconStr);
}

void TestQFluent::testIconTypeEnum_data()
{
    QTest::addColumn<Fluent::IconType>("icon");
    QTest::addColumn<QString>("iconStr");

    QTest::newRow("ADD")    << Fluent::IconType::ADD    << "ADD";
    QTest::newRow("DELETE") << Fluent::IconType::DELETE << "DELETE";
    QTest::newRow("EDIT")   << Fluent::IconType::EDIT   << "EDIT";
    QTest::newRow("SAVE")   << Fluent::IconType::SAVE   << "SAVE";
    QTest::newRow("CANCEL") << Fluent::IconType::CANCEL << "CANCEL";
    QTest::newRow("SETTING")<< Fluent::IconType::SETTING<< "SETTING";
    QTest::newRow("INFO")   << Fluent::IconType::INFO   << "INFO";
    QTest::newRow("SEARCH") << Fluent::IconType::SEARCH << "SEARCH";
    QTest::newRow("PLAY")   << Fluent::IconType::PLAY   << "PLAY";
    QTest::newRow("PAUSE")  << Fluent::IconType::PAUSE  << "PAUSE";
}

void TestQFluent::testFluentIconUtils_iconColor()
{
    // Test iconColor returns valid color strings
    QString lightColor = FluentIconUtils::iconColor(Fluent::ThemeMode::LIGHT);
    QString darkColor  = FluentIconUtils::iconColor(Fluent::ThemeMode::DARK);

    QVERIFY(!lightColor.isEmpty());
    QVERIFY(!darkColor.isEmpty());

    // Colors should be different for different themes
    // (unless explicitly same, but generally they differ)
    // Just verify they are valid color strings (hex format)
    QVERIFY(lightColor.startsWith("#") || lightColor.startsWith("rgb"));
}

// ---- Color tests ----

void TestQFluent::testColorParsing()
{
    QFETCH(QString, colorStr);
    QFETCH(bool, valid);

    QColor color(colorStr);
    QCOMPARE(color.isValid(), valid);
}

void TestQFluent::testColorParsing_data()
{
    QTest::addColumn<QString>("colorStr");
    QTest::addColumn<bool>("valid");

    QTest::newRow("valid_hex")       << "#FF5733"      << true;
    QTest::newRow("valid_hex_short") << "#F00"         << true;
    QTest::newRow("valid_named")     << "red"          << true;
    QTest::newRow("valid_named_2")   << "transparent"  << true;
    QTest::newRow("valid_rgb")       << "rgb(255,0,0)"<< true;
    QTest::newRow("invalid")         << "notacolor"    << false;
    QTest::newRow("empty")           << ""             << false;
}

QTEST_MAIN(TestQFluent)
#include "test_qfluent.moc"
