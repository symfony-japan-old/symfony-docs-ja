.. index::
   single: Security; Target redirect path

デフォルトターゲットパスの挙動を変更する方法
==============================================

デフォルトのセキュリティコンポーネントでは、セッション内に ``_security.target_path`` の値として最後にリクエストのあった URI の情報を保有しています。ログインに成功すると、ユーザはこのパスにリダイレクトされ、最後に訪れたページに戻ることができます。

しかし、これは予期せぬことを招くときもあります。例えば、最後にリクエストのあった URI が POST メソッドのみしか受け付けない HTTP POST のルートだった場合は、ユーザは 404 エラーページにリダイレクトされてしまいます。

この挙動に対処するには、 ``ExceptionListener`` クラスを拡張して、 ``setTargetPath()`` メソッドをオーバーライドする必要があります。

まず、コンフィギュレーションファイルの ``security.exception_listener.class`` パラメターをオーバーライドします。メインコンフィギュレーションやバンドルからインポートされるコンフィギュレーションファイルでオーバーライドをします。

.. configuration-block::

        .. code-block:: yaml

            # src/Acme/HelloBundle/Resources/config/services.yml
            parameters:
                # ...
                security.exception_listener.class: Acme\HelloBundle\Security\Firewall\ExceptionListener

        .. code-block:: xml

            <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
            <parameters>
                <!-- ... -->
                <parameter key="security.exception_listener.class">Acme\HelloBundle\Security\Firewall\ExceptionListener</parameter>
            </parameters>

        .. code-block:: php

            // src/Acme/HelloBundle/Resources/config/services.php
            // ...
            $container->setParameter('security.exception_listener.class', 'Acme\HelloBundle\Security\Firewall\ExceptionListener');

次に、独自の ``ExceptionListener`` を作成します
::

    // src/Acme/HelloBundle/Security/Firewall/ExceptionListener.php
    namespace Acme\HelloBundle\Security\Firewall;

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\Security\Http\Firewall\ExceptionListener as BaseExceptionListener;

    class ExceptionListener extends BaseExceptionListener
    {
        protected function setTargetPath(Request $request)
        {
            // Do not save target path for XHR and non-GET requests
            // You can add any more logic here you want
            if ($request->isXmlHttpRequest() || 'GET' !== $request->getMethod()) {
                return;
            }

            $request->getSession()->set('_security.target_path', $request->getUri());
        }
    }

ここで処理を追加するだけで、デフォルトのターゲットパスの変更が可能になりました。

.. 2011/11/08 ganchiku 95d56bc5055ca60a90293a5c960a076d8a143d62

