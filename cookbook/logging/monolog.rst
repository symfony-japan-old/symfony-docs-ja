.. index::
   single: Logging

ロギングで Monolog を使用する方法
================================

Monolog_ は、 Symfony2 で採用している ロギングを行うライブラリです。 Monolog は、 Python の LogBook ライブラリにインスパイアされて作成されました。

使用方法
-----

Monolog では、ロガーはそれぞれのロギングチャネルを定義します。そのチャネルは、多くのログを書くハンドラを持っています(ハンドラは、共有されます)。

.. tip::

    サービスでロガーを注入する際に、 :ref:`use a custom channel<dic_tags-monolog>` を参照して、ログを書かれたアプリケーションの部分を簡単に調べることができます。

ベースとなるハンドラは、 ``StreamHandler`` で、ストリーム内でログを書きます。デフォルトでは、本番環境では、 ``app/logs/prod.log`` へ、開発環境では、 ``app/logs/dev.log`` になります。

Monolog は、 ``FingersCrossedHandler`` と呼ばれる本番環境でのログ銀のための、強力なビルトインされたハンドラも付属しています。このハンドラを使用すれば、他のハンドラへのメッセージを転送することによって、メッセージがアクションレベルに到達したときのみ、バッファにメッセージを格納してそのメッセージをログに書くことができます。アクションレベルとは、 Standard edtion で提供されているコンフィギュレーション内の ERROR のことです。

メッセージをログに書くには、コントローラ内でコンテナからロガーサービスを取得(get)するだけです。::

    $logger = $this->get('logger');
    $logger->info('We just got the logger');
    $logger->err('An error occurred');

.. tip::

    :class:`Symfony\\Component\\HttpKernel\\Log\\LoggerInterface` インタフェースで定義されているメソッドのみを使用すれば、あなたのコードを一切変更しなくても、ロガーの実装を変更することができます。

いろんなハンドラの使用
~~~~~~~~~~~~~~~~~~~~~~

ロガーは、連続して呼ばれるたくさんのハンドラを使います。つまり、開発者が簡単にいくつもの方法でメッセージをログに書き込めるようになっています。

.. configuration-block::

    .. code-block:: yaml

        monolog:
            handlers:
                syslog:
                    type: stream
                    path: /var/log/symfony.log
                    level: error
                main:
                    type: fingerscrossed
                    action_level: warning
                    handler: file
                file:
                    type: stream
                    level: debug

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/monolog http://symfony.com/schema/dic/monolog/monolog-1.0.xsd">

            <monolog:config>
                <monolog:handler
                    name="syslog"
                    type="stream"
                    path="/var/log/symfony.log"
                    level="error"
                />
                <monolog:handler
                    name="main"
                    type="fingerscrossed"
                    action-level="warning"
                    handler="file"
                />
                <monolog:handler
                    name="file"
                    type="stream"
                    level="debug"
                />
            </monolog:config>
        </container>

上記のコンフィギュレーションでは、多くのハンドラを定義しています。また、このハンドラは、定義した順番で呼ばれます。

.. tip::

    "file" と名付けられたハンドラは、fingercrossed ハンドラの入れ子のハンドラとして使用されているので、スタックには入れられません。

.. note::

    他の設定ファイルで MonologBundle の設定を変更したければ、全てのスタックを再定義しなければなりません。定義では、順番が重要であり、マージでは順番を制御することができません。

Formatter の変更
~~~~~~~~~~~~~~~~~~~~~~

ハンドラは、 ``Formatter`` を使用して、実際のロギングの前にフォーマットを指定します。全ての Monologo のハンドラは、デフォルトでは ``Monolog`Formatter`LineFormatter`` のインスタンスを使用します。しかし、これの置き換えは簡単です。置き換える Formatter には ``Monolog\Formatter\FormatterInterface`` を実装する必要があります。

.. configuration-block::

    .. code-block:: yaml

        services:
            my_formatter:
                class: Monolog\Formatter\JsonFormatter
        monolog:
            handlers:
                file:
                    type: stream
                    level: debug
                    formatter: my_formatter

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/monolog http://symfony.com/schema/dic/monolog/monolog-1.0.xsd">

            <services>
                <service id="my_formatter" class="Monolog\Formatter\JsonFormatter" />
            </services>
            <monolog:config>
                <monolog:handler
                    name="file"
                    type="stream"
                    level="debug"
                    formatter="my_formatter"
                />
            </monolog:config>
        </container>

ログメッセージにデータを追加する
------------------------------------------

Monolog では、ロギングをする前にデータを追加して書き込み処理をすることができます。この Processor は、全てのハンドラスタックにも、特定のハンドラにも適用が可能です。

Processor は、第一引数として書き込むレコードを受け取るだけです(A processor is simply a callable receiving the record as it's first argument.)

このProcessor は、 ``monolog.processor`` DIC タグを使用して設定します。 :ref:`reference about it<dic_tags-monolog-processor>` を参照してください。

Session/Request のトークンの追加
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ログのエントリが、 Session か、 Request またはその両方に属しているのかを区別することが難しいときがあります。以下の例では、Processor を使用して、ユニークなトークンを個々のリクエストに追加しています。

.. code-block:: php

    namespace Acme\MyBundle;

    use Symfony\Component\HttpFoundation\Session;

    class SessionRequestProcessor
    {
        private $session;
        private $token;

        public function __construct(Session $session)
        {
            $this->session = $session;
        }

        public function processRecord(array $record)
        {
            if (null === $this->token) {
                try {
                    $this->token = substr($this->session->getId(), 0, 8);
                } catch (\RuntimeException $e) {
                    $this->token = '????????';
                }
                $this->token .= '-' . substr(uniqid(), -8);
            }
            $record['extra']['token'] = $this->token;

            return $record;
        }
    }

.. configuration-block::

    .. code-block:: yaml

        services:
            monolog.formatter.session_request:
                class: Monolog\Formatter\LineFormatter
                arguments:
                    - "[%%datetime%%] [%%extra.token%%] %%channel%%.%%level_name%%: %%message%%\n"

            monolog.processor.session_request:
                class: Acme\MyBundle\SessionRequestProcessor
                arguments:  [ @session ]
                tags:
                    - { name: monolog.processor, method: processRecord }

        monolog:
            handlers:
                main:
                    type: stream
                    path: %kernel.logs_dir%/%kernel.environment%.log
                    level: debug
                    formatter: monolog.formatter.session_request

.. note::

    ハンドラを複数使用する際に、 Processor をグローバルに追加するのではなく、ぞれのハンドラに登録することもできます。

.. _Monolog: https://github.com/Seldaek/monolog
