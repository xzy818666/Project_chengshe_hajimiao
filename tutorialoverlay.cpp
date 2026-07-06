#include "tutorialoverlay.h"
#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QDialog>

TutorialOverlay::TutorialOverlay(QWidget *parent)
    : QWidget(parent)
    , m_hasHighlight(false)
    , m_dialogMode(false)
    , m_mainWindow(parent)
    , m_tutorialActive(false)
{
    hide();
    setupUI();
}

void TutorialOverlay::setupUI()
{
    // 气泡窗口
    m_bubble = new QWidget(this);
    m_bubble->setStyleSheet(
        "QWidget {"
        "  background-color: rgba(25, 18, 10, 0.96);"
        "  border: 2px solid rgba(200, 160, 80, 0.8);"
        "  border-radius: 16px;"
        "}"
    );

    // 阴影效果
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(m_bubble);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 180));
    shadow->setOffset(0, 4);
    m_bubble->setGraphicsEffect(shadow);

    // 气泡总布局：左侧头像 + 右侧内容
    QHBoxLayout* bubbleMainLayout = new QHBoxLayout(m_bubble);
    bubbleMainLayout->setContentsMargins(16, 14, 16, 14);
    bubbleMainLayout->setSpacing(12);

    // ===== 左侧：头像区域 =====
    QWidget* avatarContainer = new QWidget(m_bubble);
    avatarContainer->setFixedWidth(100);
    QVBoxLayout* avatarLayout = new QVBoxLayout(avatarContainer);
    avatarLayout->setContentsMargins(0, 0, 0, 0);
    avatarLayout->setSpacing(4);
    avatarLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    // 头像图片
    m_avatarLabel = new QLabel(avatarContainer);
    m_avatarLabel->setFixedSize(84, 84);
    m_avatarLabel->setScaledContents(true);
    m_avatarLabel->setStyleSheet(
        "border: 2px solid rgba(200, 160, 80, 0.6);"
        "border-radius: 8px;"
        "background-color: rgba(40, 30, 20, 0.8);"
    );
    avatarLayout->addWidget(m_avatarLabel, 0, Qt::AlignHCenter);

    // 说话人名字（头像下方）
    m_speakerLabel = new QLabel("玄机子", avatarContainer);
    m_speakerLabel->setStyleSheet(
        "font-size: 14px; font-weight: bold; color: #FFD700;"
        "font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', 'SimSun', serif;"
        "border: none; background: transparent;"
    );
    m_speakerLabel->setAlignment(Qt::AlignCenter);
    avatarLayout->addWidget(m_speakerLabel);

    bubbleMainLayout->addWidget(avatarContainer);

    // ===== 右侧：文本 + 按钮 =====
    QVBoxLayout* contentLayout = new QVBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(10);

    // 步骤指示器（右上角）
    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->addStretch();
    m_stepIndicator = new QLabel("1 / 20", m_bubble);
    m_stepIndicator->setStyleSheet("font-size: 11px; color: #888888; border: none; background: transparent;");
    headerLayout->addWidget(m_stepIndicator);
    contentLayout->addLayout(headerLayout);

    // 分隔线
    QFrame* line = new QFrame(m_bubble);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: rgba(200, 160, 80, 0.4); border: none;");
    line->setFixedHeight(2);
    contentLayout->addWidget(line);

    // 文本内容
    m_textLabel = new QLabel(m_bubble);
    m_textLabel->setWordWrap(true);
    m_textLabel->setTextFormat(Qt::RichText);
    m_textLabel->setOpenExternalLinks(false);
    m_textLabel->setStyleSheet(
        "font-size: 15px; color: #F5F5DC; line-height: 1.7;"
        "font-family: 'STKaiti', 'KaiTi', 'SimSun', serif;"
        "border: none; background: transparent;"
    );
    m_textLabel->setMinimumHeight(60);
    contentLayout->addWidget(m_textLabel);

    // 底部按钮
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    m_skipBtn = new QPushButton("跳过", m_bubble);
    m_skipBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: transparent;"
        "  color: #888888;"
        "  border: 1px solid rgba(100, 100, 100, 0.4);"
        "  border-radius: 6px;"
        "  padding: 5px 14px;"
        "  font-family: 'STKaiti', 'KaiTi', 'SimSun', serif;"
        "  font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "  color: #FFD700;"
        "  border: 1px solid rgba(255, 215, 0, 0.5);"
        "}"
    );
    m_skipBtn->setCursor(Qt::PointingHandCursor);

    m_nextBtn = new QPushButton("继续 →", m_bubble);
    m_nextBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #5D4037;"
        "  color: #FFD700;"
        "  border: 2px solid #FFD700;"
        "  border-radius: 8px;"
        "  padding: 6px 20px;"
        "  font-weight: bold;"
        "  font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', 'SimSun', serif;"
        "  font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #FFD700;"
        "  color: #3E2723;"
        "}"
    );
    m_nextBtn->setCursor(Qt::PointingHandCursor);

    btnLayout->addWidget(m_skipBtn);
    btnLayout->addWidget(m_nextBtn);
    contentLayout->addLayout(btnLayout);

    bubbleMainLayout->addLayout(contentLayout, 1);

    // 箭头标签（仅主界面模式使用）
    m_arrowLabel = new QLabel(this);
    m_arrowLabel->setText("▼");
    m_arrowLabel->setStyleSheet("font-size: 24px; color: rgba(200, 160, 80, 0.9);");
    m_arrowLabel->hide();

    // 连接信号
    connect(m_nextBtn, &QPushButton::clicked, this, &TutorialOverlay::onNextClicked);
    connect(m_skipBtn, &QPushButton::clicked, this, &TutorialOverlay::onSkipClicked);

    // 自动推进定时器
    m_autoTimer = new QTimer(this);
    m_autoTimer->setSingleShot(true);
    connect(m_autoTimer, &QTimer::timeout, this, &TutorialOverlay::onAutoAdvance);

    // 连接 TutorialManager
    connect(TutorialManager::instance(), &TutorialManager::stepChanged,
            this, &TutorialOverlay::onStepChanged);
    connect(TutorialManager::instance(), &TutorialManager::tutorialFinished,
            this, &TutorialOverlay::endTutorial);
    connect(TutorialManager::instance(), &TutorialManager::tutorialSkipped,
            this, &TutorialOverlay::endTutorial);
    connect(TutorialManager::instance(), &TutorialManager::dialogOpened,
            this, &TutorialOverlay::attachToDialog);
    connect(TutorialManager::instance(), &TutorialManager::dialogClosed,
            this, &TutorialOverlay::detachToMainWindow);
}

QString TutorialOverlay::avatarPath(const QString& expression) const
{
    QString base = ":/images/tutorial_avatar_";
    if (expression == "happy") return base + "happy.png";
    if (expression == "surprised") return base + "surprised.png";
    if (expression == "serious") return base + "serious.png";
    if (expression == "worried") return base + "worried.png";
    return base + "normal.png";
}

void TutorialOverlay::startTutorial()
{
    m_tutorialActive = true;
    show();
    raise();
    TutorialManager::instance()->startTutorial();
}

void TutorialOverlay::endTutorial()
{
    m_tutorialActive = false;
    m_autoTimer->stop();
    hide();
}

void TutorialOverlay::setHighlightRect(const QRect& rect)
{
    m_highlightRect = rect;
    m_hasHighlight = rect.isValid();
    update();
    positionBubble();
    if (m_hasHighlight) {
        m_arrowLabel->show();
    } else {
        m_arrowLabel->hide();
    }
}

void TutorialOverlay::clearHighlight()
{
    m_hasHighlight = false;
    m_arrowLabel->hide();
    update();
}

void TutorialOverlay::updateStep(const TutorialManager::Step* step)
{
    if (!step) return;

    // 更新头像
    QString avatar = avatarPath(step->expression);
    QPixmap pix(avatar);
    if (!pix.isNull()) {
        m_avatarLabel->setPixmap(pix);
    } else {
        // 占位：显示首字
        m_avatarLabel->setText(step->speaker.left(1));
        m_avatarLabel->setAlignment(Qt::AlignCenter);
        m_avatarLabel->setStyleSheet(
            "font-size: 36px; font-weight: bold; color: #FFD700;"
            "border: 2px solid rgba(200, 160, 80, 0.6);"
            "border-radius: 8px;"
            "background-color: rgba(40, 30, 20, 0.8);"
        );
    }

    m_speakerLabel->setText(step->speaker);
    m_textLabel->setText(step->text);
    m_stepIndicator->setText(QString("%1 / %2")
        .arg(step->id + 1)
        .arg(TutorialManager::instance()->totalSteps()));

    // 根据步骤类型决定交互模式
    bool needUserAction = (step->waitFor == "click_area" || step->waitFor == "click_cloud");
    if (needUserAction) {
        // 需要用户点击特定区域：让事件穿透到下方控件
        setAttribute(Qt::WA_TransparentForMouseEvents, true);
        m_nextBtn->setText("等待操作...");
        m_nextBtn->setEnabled(false);
    } else {
        // 纯说明/自动推进步骤：拦截事件，允许点击气泡按钮
        setAttribute(Qt::WA_TransparentForMouseEvents, false);
        m_nextBtn->setText("继续 →");
        m_nextBtn->setEnabled(true);
    }

    // 自动推进
    if (step->autoAdvance && step->autoDelayMs > 0) {
        m_autoTimer->start(step->autoDelayMs);
    } else {
        m_autoTimer->stop();
    }

    // 重新布局气泡
    positionBubble();
    animateIn();

    // 确保覆盖层在最顶层
    raise();
    show();
}

void TutorialOverlay::positionBubble()
{
    if (m_dialogMode) {
        // 对话框模式：气泡放在对话框底部中央，只占底部一小条
        m_bubble->adjustSize();
        int bubbleW = qMin(640, parentWidget()->width() - 40);
        int bubbleH = m_bubble->height();

        int x = (parentWidget()->width() - bubbleW) / 2;
        int y = parentWidget()->height() - bubbleH - 15;

        // 调整 TutorialOverlay 自身大小为气泡大小（不覆盖整个对话框）
        setFixedSize(bubbleW, bubbleH);
        move(x, y);

        m_bubble->setGeometry(0, 0, bubbleW, bubbleH);
        m_arrowLabel->hide();
        return;
    }

    // 主界面模式：覆盖全屏，气泡定位在高亮区域附近或居中
    setFixedSize(parentWidget()->size());
    move(0, 0);

    int bubbleW = qMin(520, width() - 40);
    m_bubble->setFixedWidth(bubbleW);
    m_bubble->adjustSize();
    int bubbleH = m_bubble->height();

    int x, y;
    if (m_hasHighlight) {
        // 气泡放在高亮区域下方或上方
        int belowY = m_highlightRect.bottom() + 30;
        int aboveY = m_highlightRect.top() - bubbleH - 30;

        if (belowY + bubbleH <= height()) {
            y = belowY;
            m_arrowLabel->setText("▲");
            m_arrowLabel->move(m_highlightRect.center().x() - 12, m_highlightRect.bottom() + 5);
            m_arrowLabel->show();
        } else {
            y = qMax(10, aboveY);
            m_arrowLabel->setText("▼");
            m_arrowLabel->move(m_highlightRect.center().x() - 12, m_highlightRect.top() - 25);
            m_arrowLabel->show();
        }
        x = qMax(10, qMin(width() - bubbleW - 10, m_highlightRect.center().x() - bubbleW / 2));
    } else {
        // 居中偏下
        x = (width() - bubbleW) / 2;
        y = height() * 0.62;
        m_arrowLabel->hide();
    }

    m_bubble->move(x, y);
}

void TutorialOverlay::animateIn()
{
    m_bubble->show();
    m_bubble->raise();
}

void TutorialOverlay::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    // 对话框模式不绘制任何遮罩
    if (m_dialogMode) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_hasHighlight) {
        QPainterPath mask;
        mask.addRect(rect());

        QPainterPath hole;
        QRect highlight = m_highlightRect.adjusted(-8, -8, 8, 8);
        hole.addRoundedRect(highlight, 12, 12);

        QPainterPath finalPath = mask.subtracted(hole);
        painter.fillPath(finalPath, QColor(0, 0, 0, 170));

        painter.setPen(QPen(QColor(255, 215, 0, 200), 3));
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(highlight, 12, 12);

        QColor pulseColor(255, 215, 0, 80);
        painter.setPen(QPen(pulseColor, 6));
        painter.drawRoundedRect(highlight.adjusted(-4, -4, 4, 4), 14, 14);
    } else {
        painter.fillRect(rect(), QColor(0, 0, 0, 140));
    }
}

void TutorialOverlay::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    if (m_tutorialActive) {
        positionBubble();
    }
}

void TutorialOverlay::onStepChanged(int stepIndex)
{
    Q_UNUSED(stepIndex)
    const TutorialManager::Step* step = TutorialManager::instance()->currentStep();
    if (!step) return;

    updateStep(step);
}

void TutorialOverlay::onNextClicked()
{
    TutorialManager::instance()->nextStep();
}

void TutorialOverlay::onSkipClicked()
{
    TutorialManager::instance()->skipTutorial();
}

void TutorialOverlay::onAutoAdvance()
{
    if (m_tutorialActive) {
        TutorialManager::instance()->nextStep();
    }
}

void TutorialOverlay::attachToDialog(QDialog* dialog)
{
    if (!dialog) return;
    if (!m_mainWindow) {
        m_mainWindow = parentWidget();
    }

    m_dialogMode = true;
    setParent(dialog);

    // 对话框模式下不覆盖整个对话框，只显示气泡
    setWindowFlags(Qt::Widget);
    setAttribute(Qt::WA_TransparentForMouseEvents, false);

    raise();
    show();

    if (m_tutorialActive) {
        positionBubble();
    }
}

void TutorialOverlay::detachToMainWindow()
{
    if (m_mainWindow && parentWidget() != m_mainWindow) {
        m_dialogMode = false;
        setParent(m_mainWindow);
        setFixedSize(m_mainWindow->size());
        move(0, 0);

        setAttribute(Qt::WA_TransparentForMouseEvents, false);

        raise();
        show();

        if (m_tutorialActive) {
            positionBubble();
        }
    }
}
