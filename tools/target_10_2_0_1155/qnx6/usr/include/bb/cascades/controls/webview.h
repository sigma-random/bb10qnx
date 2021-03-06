/* Copyright (C) 2012-2013 Research In Motion Limited. Proprietary and confidential. */

#ifndef cascades_webview_h
#define cascades_webview_h

#include <QUrl>
#include <QVariantMap>

#include <bb/cascades/bbcascades_global.h>
#include <bb/cascades/controls/control.h>
#include <bb/cascades/web/javascriptworld.h>
#include <bb/cascades/web/webfindflag.h>

namespace bb {
    namespace cascades {

    class ActionSet;
    class WebPage;
    class WebResourceRequestFilter;
    class WebViewPrivate;
    class WebSettings;
    class Container;
    class WebNavigationRequest;
    class WebLoadRequest;
    class WebStorage;

   /*!
    * @brief A control that is used to display dynamic web content.
    *
    * The @c %WebView control supports loading content from the web as well as web content defined in QML
    * or bundled with your application.
    *
    * Content is always loaded asynchronously. The progress of loading content is reported through
    * the @c loadingChanged(), @c loadProgressChanged(), and @c urlChanged()
    * signals. The @c #loading, @c #loadProgress, and @c #url properties can be used to get the current
    * values. Note that the webpage is not considered to be in the loading state until it has
    * received a response from the server.
    *
    * Normally, the user can interact with the @c %WebView, and JavaScript code can execute on the web
    * page. This can cause the @c %WebView to change appearance, or navigate to other
    * locations.
    *
    * You can interact with JavaScript executing on the page by using @c postMessage() and connecting
    * to the @c messageReceived() signal. JavaScript can communicate back by using the @c navigator.cascades.postMessage()
    * function and storing a message handler in the @c navigator.cascades.onmessage property.
    *
    * You can also inject JavaScript code directly into the webpage using @c evaluateJavaScript(). Note that
    * the script executes in an "isolated world", and will normally leave no trace detectable by
    * the webpage. For example, properties you add to the global object will not be seen by the
    * JavaScript code executing in the "normal world" as part of the webpage.
    *
    * To control navigation that occurs while the @c %WebView displays a webpage, connect to the
    * @c navigationRequested() signal.
    *
    * In order to make the WebView scrollable, you can place it in a @c ScrollView. To enable pinch to zoom,
    * place the @c WebView in a @c ScrollView, and set the associated @c ScrollViewProperties @c pinchToZoomEnabled
    * property to @c true. The web content may suggest a minimum and maximum content scale. To enforce them,
    * connect the @c WebView's @c min/maxContentScaleChanged() signals to the @c ScrollViewProperties
    * @c setMin/MaxContentScale() slots. In QML, hook up the @c onMin/MaxContentScaleChanged() notification to set
    * the corresponding property on the @c ScrollView's @c scrollViewProperties object.
    *
    * For advanced use cases, implementing the @c WebResourceRequestFilter interface and calling
    * @c setResourceRequestFilter() will give fine-grained control over
    * resource loading.
    *
    * @section web_view_examples Examples
    *
    * Here's an example of specifying HTML code directly in the QML file:
    *
    * @snippet tad/examples/controls/webview/assets/main.qml webviewhtml_qml1
    *
    * Here's an example of loading local assets bundled with the application:
    *
    * @snippet tad/examples/controls/webview/assets/main.qml webviewlocalurl_qml1
    *
    * Here's an example of setting up two-way communication between Cascades and JavaScript code
    * executing on the webpage:
    *
    * @snippet tad/examples/controls/webview/assets/main.qml webviewjs_qml1
    *
    * Here's an example of placing a @c WebView in a @c ScrollView in order to make it scrollable and zoomable.
    *
    * @snippet tad/examples/controls/webview/assets/main.qml webviewscrollable_qml1
    *
    * @xmlonly
    * <qml>
    * <class abstract="yes" />
    * </qml>
    * <apigrouping group="User interface/Web"/>
    * @endxmlonly
    *
    * @since BlackBerry 10.0.0
    */
    class BBCASCADES_EXPORT WebView : public Control
    {
        Q_OBJECT

       /*!
        * @brief The @c WebSettings object associated with this @c WebView.
        *
        * @xmlonly
        * <qml>
        * <property groupedProperty="yes"/>
        * </qml>
        * @endxmlonly
        *
        * @since BlackBerry 10.0.0
        */
        Q_PROPERTY(bb::cascades::WebSettings* settings READ settings CONSTANT)

       /*!
        * @brief The @c WebStorage object associated with this @c WebView.
        *
        * The @c WebStorage class provides an API for the caching and storage
        * that the @c WebView generates internally during browsing operations. It can be used
        * to clear those caches and delete all other accumulated information.
        *
        * @xmlonly
        * <qml>
        * <property groupedProperty="yes"/>
        * </qml>
        * @endxmlonly
        *
        * @since BlackBerry 10.0.0
        */
        Q_PROPERTY(bb::cascades::WebStorage* storage READ storage CONSTANT)

        /*!
         * @brief The title of the currently loaded webpage.
         *
         * @since BlackBerry 10.0.0
         */
        Q_PROPERTY(QString title READ title NOTIFY titleChanged FINAL);

        /*!
        * @brief The current URL for the view.
        *
        * It can be set, which is identical to calling load with the same URL.
        * It can also spontaneously change as a result of redirects or JavaScript.
        *
        * @param url The URL of the current webpage.
         *
         * @since BlackBerry 10.0.0
        */
        Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged FINAL)

        /*!
        * @brief The URL for the favicon for the current page, if any.
         *
         * @since BlackBerry 10.0.0
        */
        Q_PROPERTY(QUrl icon READ icon NOTIFY iconChanged FINAL)

        /*!
        * @brief Whether the navigation history contains a previous page.
        *
        * @see #goBack()
         *
         * @since BlackBerry 10.0.0
        */
        Q_PROPERTY(bool canGoBack READ canGoBack NOTIFY navigationHistoryChanged FINAL)

        /*!
        * @brief Whether the navigation history contains a next page.
        *
        * @see #goForward()
         *
         * @since BlackBerry 10.0.0
        */
        Q_PROPERTY(bool canGoForward READ canGoForward NOTIFY navigationHistoryChanged FINAL)

       /*!
        * @brief This property holds any HTML that was specifically set as a QString.
        *
        * @param html The HTML to be set. This property only changes when set by
        * the user, regardless of what content is actually loaded subsequently.
        *
        * @since BlackBerry 10.0.0
        */
        Q_PROPERTY(QString html READ html WRITE setHtml FINAL)

       /*!
        * @brief True if the web view is currently loading a webpage, false otherwise.
        *
        * @since BlackBerry 10.0.0
        */
        Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged FINAL)

        /*!
         * @brief The progress of loading the current webpage.
         *
         * The range is from 0 to 100.
         *
         * @since BlackBerry 10.0.0
         */
        Q_PROPERTY(int loadProgress READ loadProgress NOTIFY loadProgressChanged)

        /*!
         * @brief Suggested maximum content scale factor for pinch zoom.
         *
         * The value of this property can be specified by web content. In order
         * to apply the suggested value, place the @c WebView in a @c ScrollView that
         * has pinch to zoom enabled, and connect the @c maxContentScaleChanged() signal
         * to @c setMaxContentScale().
         *
         * @return Suggested maximum scale factor allowed.
         *
         * @see @c setMaxContentScale() and @c setPinchToZoomEnabled().
         *
         * @since BlackBerry 10.0.0
         */
        Q_PROPERTY(float maxContentScale READ maxContentScale NOTIFY maxContentScaleChanged FINAL)

        /*!
         * @brief Suggested minimum content scale factor for pinch zoom.
         *
         * The value of this property can be specified by web content. In order
         * to apply the suggested value, place the @c WebView in a @c ScrollView that
         * has pinch to zoom enabled, and connect the @c minContentScaleChanged() signal
         * to @c setMinContentScale().
         *
         * @return Suggested minimum scale factor allowed.
         *
         * @see @c setMinContentScale() and @c setPinchToZoomEnabled().
         *
         * @since BlackBerry 10.0.0
         */
        Q_PROPERTY(float minContentScale READ minContentScale NOTIFY minContentScaleChanged FINAL)

    public:

       /*!
        * @brief Constructs a @c WebView with a parent.
        *
        * If not 0, the ownership of the constructed @c WebView will be transferred to the parent.
        *
        * @param parent The parent container or 0. Optional, and will default to 0 if not specified.
        *
        * @since BlackBerry 10.0.0
        */
        explicit WebView(Container *parent = 0);

        /*!
         * @brief Constructs a @c WebView from an existing @c WebPage object.
         *
         * The @c WebView will not take ownership of the @c WebPage. If ownership is desired, the caller
         * must call @c page->setParent(), passing in the newly constructed @c WebView.
         *
         * If not 0, the ownership of the constructed @c WebView will be transferred to the parent.
         *
         * @param page The @c WebPage object whose content should be displayed in this @c WebView.
         * @param parent The parent container or 0. Optional, and will default to 0 if not specified.
         */
        explicit WebView(WebPage* page, Container* parent = 0);

        /*!
        * @brief Constructs a @c WebView as a child of another @c WebView.
        *
        * Ownership of the constructed @c WebView is transferred to the parent.
        *
        * @param parent The parent @c WebView.
         *
         * @since BlackBerry 10.0.0
        */
        explicit WebView(WebView *parent);

        virtual ~WebView();

       /*!
        * @brief Returns the settings object for this @c WebView control.
        *
        * @see @c WebSettings.
        *
        * @since BlackBerry 10.0.0
        */
        bb::cascades::WebSettings *settings() const;

       /*!
        * @brief Returns the @c WebStorage object for this @c WebView control.
        *
        * @see @c WebStorage.
        *
        * @since BlackBerry 10.0.0
        */
        bb::cascades::WebStorage *storage() const;

        /*!
         * @brief Returns @c true if there's a previous page in the navigation history.
         *
         * @since BlackBerry 10.0.0
         */
        bool canGoBack() const;

        /*!
         * @brief Returns @c true if there's a next page in the navigation history.
         *
         * @since BlackBerry 10.0.0
         */
        bool canGoForward() const;

        /*!
         * @brief Retrieves the loading state of the current webpage.
         *
         * @return @c true if the WebView is currently loading a webpage, @c false otherwise.
         *
         * @since BlackBerry 10.0.0
         */
        bool loading() const;

        /**
         * @brief Retrieves the progress of loading the current webpage.
         *
         * The range is from 0 to 100.
         *
         * @return Progress as an integer value ranging from 0 to 100.
         * @see @c ProgressIndicator
         *
         * @since BlackBerry 10.0.0
         */
        int loadProgress() const;

        /*!
         * @brief Evaluates the specified JavaScript.
         *
         * Note: A @c javaScriptResult will be emitted as a result of this
         *       asynchronous operation.
         *
         * @param script The JavaScript code to run.
         * @param world The JavaScript world in which the code should run. If left
         *             unspecified, @c Normal is the default.
         * @return A token that can be kept to match up the result of this @c evaluateJavaScript
         *             with a subsequent @c javaScriptResult.
         *
         * @see @c javaScriptResult
         *
         * @since BlackBerry 10.0.0
         */
        Q_INVOKABLE int evaluateJavaScript(const QString& script, bb::cascades::JavaScriptWorld::Type world = bb::cascades::JavaScriptWorld::Normal);

        /*!
         * @brief Returns the title of the currently loaded webpage.
         *
         * @since BlackBerry 10.0.0
         */
        QString title() const;

        /*!
         * @brief Retrieves the URL of the current webpage.
         *
         * It can change at any time due to JavaScript or redirects.
         *
         * @return a QUrl of the current page.
         *
         * @since BlackBerry 10.0.0
         */
        QUrl url() const;

        /*!
         * @brief Sets a URL of a page to load.
         *
         * @param url The URL to load.
         *
         * @since BlackBerry 10.0.0
         */
        void setUrl(const QUrl& url);

        /*!
         * @brief Retrieves the URL of the favicon for the current page, if any.
         *
         * @since BlackBerry 10.0.0
         */
        QUrl icon() const;

        /*!
         * @brief Retrieves a previously set QString of HTML.
         *
         * This value only changes through explicit calls to setHtml. It
         * is never modified due to the content changing for other reasons.
         *
         * @return The most recently set HTML from setHtml.
         *
         * @since BlackBerry 10.0.0
         */
        QString html() const;

        /*!
         * @brief Sets the HTML to load immediately.
         *
         * If the HTML is already available in memory, this can be used
         * to bypass everything and just load a given string as HTML.
         * Note: This method is a convenience method that is analogous to
         * <code>WebView::loadData(html.toUtf8(), "text/html; charset=utf-8", baseUrl);</code>
         *
         * @param html The string to load as HTML.
         * @param baseUrl A base URL for relative path resolution.
         *
         * @since BlackBerry 10.0.0
         */
        void setHtml(const QString& html, const QUrl& baseUrl = QUrl());

        /*!
         * @brief Specifies a @c WebResourceRequestFilter for this @c WebPage.
         *
         * The @c %WebResourceRequestFilter provides a powerful API for
         * consumers to strictly control the networking requests going
         * in and out of this webpage.
         *
         * @param filter A @c WebResourceRequestFilter implementation.
         *        It can be set to null to remove a former filter.
         *
         * @see @c WebResourceRequestFilter
         *
         * @since BlackBerry 10.0.0
         */
        void setResourceRequestFilter(WebResourceRequestFilter* filter);

        /*!
         * @brief Finds a given substring within the current page.
         *
         * This method will find and highlight, but not select, text in
         * the page that matches the string that is passed in. This feature
         * is typically used to assist users in locating a specific passage
         * of interest within a webpage.
         *
         * @param subString The string to locate.
         * @param options A set of @c WebFindFlag values.
         * @see @c WebFindFlag.
         *
         * @since BlackBerry 10.0.0
         */
        Q_INVOKABLE void findText(const QString& subString, WebFindFlag::Types options = 0);

        /*!
         * @brief Retrieves the suggested minimum content scale.
         *
         * @return The suggested minimum content scale.
         *
         * @since BlackBerry 10.0.0
         */
        float minContentScale() const;

        /*!
         * @brief Retrieves the suggested maximum content scale.
         *
         * @return The suggested maximum content scale.
         *
         * @since BlackBerry 10.0.0
         */
        float maxContentScale() const;

    Q_SIGNALS:
       /*!
        * @brief Emitted when a webpage load request has changed status and the loading property
        * is changed (when loading started, finished, or failed).
        *
        * Here's a QML example demonstrating the use of a @c %WebLoadRequest:
        * @snippet tad/examples/controls/webview/assets/main.qml webviewmisc_qml1
        *
        * @param loadRequest Contains the URL and status of the load request that changed status.
        *
        * @see @c WebLoadRequest
        *
        * @since BlackBerry 10.0.0
        */
        void loadingChanged(bb::cascades::WebLoadRequest* loadRequest);

        /*!
         * @brief Emitted when the loading progress of the webpage changes.
         *
         * @param loadProgress The new progress value.
         *
         * @since BlackBerry 10.0.0
         */
        void loadProgressChanged(int loadProgress);

        /*!
         * @brief Emitted when the title of the webpage has changed.
         *
         * @param title The new title.
         *
         * @since BlackBerry 10.0.0
         */
        void titleChanged(const QString& title);

        /*!
         * @brief Emitted when navigation history has changed.
         *
         * The navigation history is considered to have changed when navigating
         * to a different page. Depending on the type of navigation, the
         * availability of the @c goBack() and @c goForward() actions may change.
         *
         * @since BlackBerry 10.0.0
         */
        void navigationHistoryChanged();

        /*!
         * @brief Emitted when the URL of the webpage has changed (after the change is committed).
         *
         * This will be emitted after @c navigationRequested().
         *
         * @param url The new URL.
         *
         * @since BlackBerry 10.0.0
         */
        void urlChanged(const QUrl& url);

        /*!
         * @brief Emitted when the URL of the favicon of the webpage has changed.
         *
         * @param icon The URL for the new icon.
         *
         * @since BlackBerry 10.0.0
         */
        void iconChanged(const QUrl& icon);

        /*!
         * @brief Emitted for every navigation request.
         *
         * The request object contains the URL
         * and type of the navigation action, for example, "a link click on 'http://www.rim.com/".
         * The navigation will be accepted by default. To reject the request, you can change
         * the action property of the @c WebNavigationRequest or call @c WebNavigationRequest::ignore()
         * In the case of OpenWindow type, accepting the request loads the URL in the current tab.
         * To open it in a new tab it must be rejected and loaded into another @c WebView independently.
         * The request object cannot be used after the signal handler function ends.
         *
         * Here's a  QML example:
         * @snippet tad/examples/controls/webview/assets/main.qml webviewmisc_qml2
         *
         * @see @c WebNavigationRequest
         *
         * @since BlackBerry 10.0.0
         */
        void navigationRequested(bb::cascades::WebNavigationRequest* request);

        /*!
         * @brief Emitted as a response to @c evaluateJavaScript().
         *
         * @param resultId A token to match up a response with a call to @c evaluateJavaScript().
         * @param result The final result of the JavaScript operation as a @c QVariant. The
         *               underlying type naturally depends on the JavaScript evaluated.
         *
         * @since BlackBerry 10.0.0
         */
        void javaScriptResult(int resultId, const QVariant& result);

        /*!
         * @brief Emitted if JavaScript code executing on the webpage is terminated because it was running for too long.
         *
         * @since BlackBerry 10.0.0
         */
        void javaScriptInterrupted();

        /*!
         * @brief Emitted when JavaScript code executing on the webpage calls
         * @c navigator.cascades.postMessage().
         *
         * @param message Contains at least two key-value pairs, a URL and
         *                the message, stored as a @c QUrl under the "origin" key
         *                and @c QString under the "data" keys, respectively.
         *
         * @since BlackBerry 10.0.0
         */
        void messageReceived(const QVariantMap& message);

        /*!
         * @brief Emitted, for example, when the caret moves within an editable text field.
         * This could be used as a notification to update context-dependent controls,
         * such as buttons used to create a rich text editor.
         *
         * @since BlackBerry 10.0.0
         */
        void microFocusChanged();

        /*!
         * @brief Emitted when the web content suggests a new value for the minimum content scale.
         *
         * @param minContentScale The new suggested value for minimum content scale.
         *
         * @since BlackBerry 10.0.0
         */
        void minContentScaleChanged(float minContentScale);

        /*!
         * @brief Emitted when the web content suggests a new value for the maximum content scale.
         *
         * @param maxContentScale The new suggested value for maximum content scale.
         *
         * @since BlackBerry 10.0.0
         */
        void maxContentScaleChanged(float maxContentScale);

    public Q_SLOTS:
        /*!
         * @copydoc WebView::setHtml()
         *
         * Note: This method is analogous to @c setHtml().
         *
         * @since BlackBerry 10.0.0
         */
        void loadHtml(const QString& html, const QUrl& baseUrl = QUrl());

        /*!
         * @brief Loads the specified data from memory.
         *
         * Note: The view will be updated asynchronously. This method returns immediately.
         *
         * @param data The data to load.
         * @param mimeType The MIME type of the data. It can be omitted if unknown.
         * @param baseUrl A base URL to resolve relative paths against. If not required it can be omitted.
         *
         * @since BlackBerry 10.0.0
         */
        void loadData(const QByteArray& data, const QString& mimeType = QString(), const QUrl& baseUrl = QUrl());

        /*!
         * Load a file, with option to override the content type (mime type).
         *
         * @param absoluteFilePath An absolute path to a file in the file system.
         * @param mimeType The MIME type of the data. It can be omitted if unknown.
         *
         * @since BlackBerry 10.0.0
         */
        void loadFile(const QString& absoluteFilePath, const QString& mimeType = QString::null);

        /*!
         * @brief Navigates to the previous page in the navigation history.
         *
         * If there's no previous page, this method does nothing.
         *
         * @since BlackBerry 10.0.0
         */
        void goBack();

        /*!
         * @brief Navigates to the next page in the navigation history.
         *
         * If there's no next page, this method does nothing.
         *
         * @since BlackBerry 10.0.0
         */
        void goForward();

        /*!
         * @brief Stops any loading in progress.
         *
         * If no loading is in progress, this method does nothing.
         *
         * @since BlackBerry 10.0.0
         */
        void stop();

        /*!
         * @brief Reloads the current page.
         *
         * @since BlackBerry 10.0.0
         */
        void reload();

        /*!
         * @brief Posts a message to an @c onmessage function registered with the
         * @c navigator.cascades object by JavaScript code executing on the page.
         *
         * @param message A string that will be sent to the @c onmessage handler
         *                registered, if any.
         *
         * @since BlackBerry 10.0.0
         */
        void postMessage(const QString& message);

    private:
       /*! @cond PRIVATE */
        Q_DECLARE_PRIVATE(WebView)
        Q_DISABLE_COPY(WebView)
       /*! @endcond */

    public:
        typedef WebView ThisClass;
        typedef Control BaseClass;

        /*!
         * @brief A builder template for constructing a @c WebView.
         * @see @c WebView::create() for getting a concrete Builder for constructing a @c WebView.
         *
         * @since BlackBerry 10.0.0
         */
        template <typename BuilderType, typename BuiltType>
        class TBuilder : public BaseClass::TBuilder<BuilderType, BuiltType>
        {
        protected:
            TBuilder(BuiltType* node) : BaseClass::TBuilder<BuilderType, BuiltType>(node)
            {
            }
        };

       /*!
        * @brief A concrete builder class for constructing a WebView.
        * @see @c WebView::create() for getting a concrete Builder for constructing a @c WebView.
        *
        * @since BlackBerry 10.0.0
        */
        class Builder : public TBuilder<Builder, WebView>
        {
        public:
            Builder() : TBuilder<Builder, WebView>(new WebView())
            {
            }
            Builder(WebView *webView) : TBuilder<Builder, WebView>(new WebView(webView))
            {
            }
            Builder(WebPage *webPage) : TBuilder<Builder, WebView>(new WebView(webPage))
            {
            }
        };

       /*!
        * @brief Creates and returns a builder for constructing a @c WebView.
        * @return A builder for constructing a @c WebView.
        *
        * @since BlackBerry 10.0.0
        */
        static Builder create()
        {
            return Builder();
        }

       /*!
        * @brief Creates and returns a builder for constructing a @c WebView as a child to an existing @c WebView.
        * @return A builder for constructing a @c WebView.
        *
        * @since BlackBerry 10.0.0
        */
        static Builder create(WebView *webView)
        {
            return Builder(webView);
        }

        /*!
         * @brief Creates and returns a builder for constructing a @c WebView from an existing @c WebPage.
         * @return A builder for constructing a @c WebView.
         */
         static Builder create(WebPage *webPage)
         {
             return Builder(webPage);
         }
    };
}
}

QML_DECLARE_TYPE(bb::cascades::WebView)

#endif	/* cascades_webview_h */
