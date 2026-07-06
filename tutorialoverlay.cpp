#include "tutorialoverlay.h"
#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QDialog>
#include <QMouseEvent>

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

TutorialOverlay::~TutorialOverlay()
{
    clearWidgetGlow();
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

    m_avatarLabel = new QLabel(avatarContainer);
    m_avatarLabel->setFixedSize(84, 84);
    m_avatarLabel->setScaledContents(true);
    m_avatarLabel->setStyleSheet(
        "border: 2px solid rgba(200, 160, 80, 0.6);"
        "border-radius: 8px;"
        "background-color: rgba(40, 30, 20, 0.8);"
    );
    avatarLayout->addWidget(m_avatarLabel, 0, Qt::AlignHCenter);

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

    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->addStretch();
    m_stepIndicator = new QLabel("1 / 20", m_bubble);
    m_stepIndicator->setStyleSheet("font-size: 11px; color: #888888; border: none; background: transparent;");
    headerLayout->addWidget(m_stepIndicator);
    contentLayout->addLayout(headerLayout);

    QFrame* line = new QFrame(m_bubble);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: rgba(200, 160, 80, 0.4); border: none;");
    line->setFixedHeight(2);
    contentLayout->addWidget(line);

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

    // 箭头标签（仅主界面模式）
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

    // 发光脉冲定时器
    m_glowTimer = new QTimer(this);
    m_glowTimer->setInterval(150);
    connect(m_glowTimer, &QTimer::timeout, this, &TutorialOverlay::updateGlowPulse);

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
    clearWidgetGlow();
    clearHighlight();
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

// ===== 目标控件金色发光效果（对话框模式） =====
void TutorialOverlay::setWidgetGlow(QWidget* target)
{
    if (!target) return;
    clearWidgetGlow();
    m_highlightTarget = target;
    m_glowEffect = new QGraphicsDropShadowEffect(target);
    m_glowEffect->setColor(QColor(255, 215, 0, 200));
    m_glowEffect->setBlurRadius(15);
    m_glowEffect->setOffset(0, 0);
    target->setGraphicsEffect(m_glowEffect);
    m_glowPhase = 0;
    m_glowTimer->start();
}

void TutorialOverlay::clearWidgetGlow()
{
    m_glowTimer->stop();
    if (m_highlightTarget && m_glowEffect) {
        m_highlightTarget->setGraphicsEffect(nullptr);
    }
    m_highlightTarget = nullptr;
    m_glowEffect = nullptr;
}

void TutorialOverlay::updateGlowPulse()
{
    if (!m_glowEffect) return;
    m_glowPhase = (m_glowPhase + 1) % 8;
    int radius = 8 + qAbs(m_glowPhase - 4) * 5;
    m_glowEffect->setBlurRadius(radius);
}

// 查找目标控件（优先找可见且足够大的，避免 .ui 中的默认小控件）
static QWidget* findTutorialTarget(QWidget* parent, const QString& name, int minW, int minH)
{
    if (!parent || name.isEmpty()) return nullptr;
    QList<QWidget*> candidates = parent->findChildren<QWidget*>(name);
    for (QWidget* c : candidates) {
        if (c->isVisible() && c->width() > minW && c->height() > minH) {
            return c;
        }
    }
    return nullptr;
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

    // 清除旧的发光
    clearWidgetGlow();

    // 主界面模式：查找高亮区域（优先找可见且足够大的控件）
    if (!m_dialogMode && !step->targetWidget.isEmpty() && parentWidget()) {
        QWidget* target = findTutorialTarget(parentWidget(), step->targetWidget, 50, 50);
        if (target) {
            QPoint globalPos = target->mapToGlobal(QPoint(0, 0));
            QPoint localPos = mapFromGlobal(globalPos);
            setHighlightRect(QRect(localPos, target->size()));
        } else {
            clearHighlight();
        }
    }

    // 对话框模式：给目标控件添加金色发光
    if (m_dialogMode && !step->targetWidget.isEmpty() && parentWidget()) {
        QWidget* target = findTutorialTarget(parentWidget(), step->targetWidget, 30, 30);
        if (target) {
            setWidgetGlow(target);
        }
    }
    // dialog_close 步骤如果没指定目标，尝试查找"返回"按钮
    else if (m_dialogMode && step->waitFor == "dialog_close" && parentWidget()) {
        QWidget* target = parentWidget()->findChild<QWidget*>("okButton");
        if (!target) target = parentWidget()->findChild<QWidget*>("closeButton");
        if (!target) target = parentWidget()->findChild<QWidget*>("backButton");
        if (target) {
            setWidgetGlow(target);
        }
    }

    // 保存当前步骤对话框名（用于气泡定位）
    m_currentStepDialog = step->targetDialog;

    // 根据步骤类型决定交互模式
    bool needUserAction = (step->waitFor == "click_area" || step->waitFor == "click_cloud");
    if (needUserAction) {
        setAttribute(Qt::WA_TransparentForMouseEvents, true);
        m_nextBtn->setText("等待操作...");
        m_nextBtn->setEnabled(false);
    } else {
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

    raise();
    show();
}

void TutorialOverlay::positionBubble()
{
    if (m_dialogMode) {
        // 对话框模式：气泡放在目标控件的对侧，避免遮挡
        if (m_bubble->layout()) {
            m_bubble->layout()->activate();
        }

        int dialogW = parentWidget()->width();
        int dialogH = parentWidget()->height();
        int bubbleW = qMin(560, dialogW - 40);
        if (bubbleW < 360) bubbleW = 360;

        m_bubble->setFixedWidth(bubbleW);
        m_bubble->adjustSize();
        int bubbleH = m_bubble->height();

        int maxH = dialogH * 0.65;
        if (bubbleH > maxH) {
            m_textLabel->setStyleSheet(
                "font-size: 13px; color: #F5F5DC; line-height: 1.5;"
                "font-family: 'STKaiti', 'KaiTi', 'SimSun', serif;"
                "border: none; background: transparent;"
            );
            m_bubble->adjustSize();
            bubbleH = m_bubble->height();
            m_textLabel->setStyleSheet(
                "font-size: 15px; color: #F5F5DC; line-height: 1.7;"
                "font-family: 'STKaiti', 'KaiTi', 'SimSun', serif;"
                "border: none; background: transparent;"
            );
        }
        if (bubbleH > maxH) bubbleH = maxH;

        int x, y;

        // 善财司 dialog_close 步骤，气泡放左下角
        if (m_currentStepDialog == "BankDialog") {
            x = 10;
            y = dialogH - bubbleH - 40;
        } else if (m_highlightTarget) {
            x = (dialogW - bubbleW) / 2;
            QPoint globalPos = m_highlightTarget->mapToGlobal(QPoint(0,0));
            QPoint dialogPos = parentWidget()->mapFromGlobal(globalPos);
            int targetCenterY = dialogPos.y() + m_highlightTarget->height() / 2;
            int midY = dialogH / 2;

            if (targetCenterY > midY) {
                y = 10;
            } else {
                y = dialogH - bubbleH - 40;
            }
        } else {
            x = (dialogW - bubbleW) / 2;
            y = 10;
        }

        if (x + bubbleW > dialogW - 20) x = dialogW - bubbleW - 20;
        if (x < 10) x = 10;
        if (y + bubbleH > dialogH - 20) y = dialogH - bubbleH - 20;
        if (y < 10) y = 10;

        int shadowPad = 10;
        setFixedSize(bubbleW + shadowPad*2, bubbleH + shadowPad*2);
        move(x - shadowPad, y - shadowPad);
        m_bubble->setGeometry(shadowPad, shadowPad, bubbleW, bubbleH);
        m_arrowLabel->hide();
        return;
    }

    // 主界面模式：覆盖全屏
    setFixedSize(parentWidget()->size());
    move(0, 0);

    int bubbleW = qMin(520, width() - 40);
    m_bubble->setFixedWidth(bubbleW);
    m_bubble->adjustSize();
    int bubbleH = m_bubble->height();

    int x, y;
    if (m_hasHighlight) {
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
        // 窗口大小变化后重新计算高亮位置
        const TutorialManager::Step* step = TutorialManager::instance()->currentStep();
        if (step && !step->targetWidget.isEmpty() && !m_dialogMode && parentWidget()) {
            QWidget* target = findTutorialTarget(parentWidget(), step->targetWidget, 50, 50);
            if (target) {
                QPoint globalPos = target->mapToGlobal(QPoint(0, 0));
                QPoint localPos = mapFromGlobal(globalPos);
                setHighlightRect(QRect(localPos, target->size()));
            }
        }
        positionBubble();
    }
}

void TutorialOverlay::onStepChanged(int stepIndex)
{
    Q_UNUSED(stepIndex)
    clearWidgetGlow();
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
        clearWidgetGlow();
        m_dialogMode = false;
        setParent(m_mainWindow);
        setFixedSize(m_mainWindow->size());
        move(0, 0);

        raise();
        show();

        if (m_tutorialActive) {
            positionBubble();
        }
    }
}
