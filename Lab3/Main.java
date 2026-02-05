import java.util.ArrayList;
import java.util.List;

// ==========================================
// 1. ПАТТЕРН: ABSTRACT FACTORY (Абстрактна фабрика)
// Створює елементи інтерфейсу залежно від ОС
// ==========================================
interface UIButton { void render(); }
interface UIWindow { void show(); }

class WinButton implements UIButton { public void render() { System.out.println("[UI] Квадратна кнопка Windows"); } }
class MacButton implements UIButton { public void render() { System.out.println("[UI] Заокруглена кнопка macOS"); } }

class WinWindow implements UIWindow { public void show() { System.out.println("[UI] Вікно у стилі Aero (Windows)"); } }
class MacWindow implements UIWindow { public void show() { System.out.println("[UI] Вікно у стилі Aqua (macOS)"); } }

interface UIFactory {
    UIButton createButton();
    UIWindow createWindow();
}

class WindowsFactory implements UIFactory {
    public UIButton createButton() { return new WinButton(); }
    public UIWindow createWindow() { return new WinWindow(); }
}

class MacOSFactory implements UIFactory {
    public UIButton createButton() { return new MacButton(); }
    public UIWindow createWindow() { return new MacWindow(); }
}

// ==========================================
// 2. ПАТТЕРН: BRIDGE (Міст)
// Розділяє "Тип сповіщення" та "Спосіб доставки"
// ==========================================
interface MessageSender { void sendMessage(String message); }

class EmailSender implements MessageSender {
    public void sendMessage(String message) { System.out.println("[Email] Надсилання: " + message); }
}

class SMSSender implements MessageSender {
    public void sendMessage(String message) { System.out.println("[SMS] Надсилання: " + message); }
}

abstract class Notification {
    protected MessageSender sender;
    protected Notification(MessageSender sender) { this.sender = sender; }
    abstract void notifyUser(String news);
}

class UrgentNotification extends Notification {
    public UrgentNotification(MessageSender sender) { super(sender); }
    public void notifyUser(String news) {
        System.out.print("!!! ТЕРМІНОВО !!! ");
        sender.sendMessage(news);
    }
}

// ==========================================
// 3. ПАТТЕРН: OBSERVER (Наглядач)
// Слідкує за зміною ціни акцій
// ==========================================
interface Observer { void update(double price); }

class StockMarket {
    private List<Observer> observers = new ArrayList<>();
    private double price;

    public void addObserver(Observer o) { observers.add(o); }
    public void setPrice(double newPrice) {
        this.price = newPrice;
        System.out.println("\n[Market] Ціна акції змінилася на: $" + price);
        for (Observer o : observers) o.update(price);
    }
}

class Investor implements Observer {
    private String name;
    private Notification notification;

    public Investor(String name, Notification notification) {
        this.name = name;
        this.notification = notification;
    }

    @Override
    public void update(double price) {
        notification.notifyUser("Інвестор " + name + ", нова ціна: $" + price);
    }
}

// ==========================================
// ГОЛОВНИЙ КЛАС
// ==========================================
public class Main {
    public static void main(String[] args) {
        // --- Демонстрація Abstract Factory ---
        System.out.println("--- Налаштування інтерфейсу ---");
        String os = "Windows"; // Можна змінити на macOS
        UIFactory uiFactory;

        if (os.equalsIgnoreCase("Windows")) {
            uiFactory = new WindowsFactory();
        } else {
            uiFactory = new MacOSFactory();
        }

        uiFactory.createWindow().show();
        uiFactory.createButton().render();

        // --- Демонстрація Bridge + Observer ---
        System.out.println("\n--- Робота системи сповіщень ---");
        StockMarket appleStock = new StockMarket();

        // Перший інвестор отримує термінові SMS
        Notification smsUrgent = new UrgentNotification(new SMSSender());
        Investor inv1 = new Investor("Олександр", smsUrgent);

        // Другий інвестор отримує звичайні Email (можна розширити клас Notification)
        Notification emailUrgent = new UrgentNotification(new EmailSender());
        Investor inv2 = new Investor("Марія", emailUrgent);

        appleStock.addObserver(inv1);
        appleStock.addObserver(inv2);

        // Зміна ціни активує ланцюжок шаблонів
        appleStock.setPrice(150.50);
        appleStock.setPrice(155.00);
    }
}