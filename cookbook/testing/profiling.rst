.. index::
   single: Tests; Profiling

ファンクショナルテストでプロファイラを使用する方法
============================================

ファンクショナルテストはレスポンスのみをテストすることを強くお勧めします。しかし、本番サーバをモニターするファンクショナルテストを書いているのであれば、データをプロファイリングしてテストを書くこともできます。そうすれば、測定を実施できますしいろんなことを調べることができるなど多くを享受できます。

Symfony2 のプロファイラ :doc:`Profiler </book/internals/profiler>`  は、毎回のリクエストに関するたくさんのデータを集めます。このデータを使ってデータベース呼び出しの回数を調べたり、フレームワーク内でかかる時間を調べることができます。しかし、アサーションを書く前に、常にプロファイラが本当に使用可能か調べてください。( ``test`` 環境のデフォルトでは有効になっています)
::

    class HelloControllerTest extends WebTestCase
    {
        public function testIndex()
        {
            $client = static::createClient();
            $crawler = $client->request('GET', '/hello/Fabien');

            // Write some assertions about the Response
            // ...

            // Check that the profiler is enabled
            if ($profile = $client->getProfile()) {
                // check the number of requests
                $this->assertTrue($profile->getCollector('db')->getQueryCount() < 10);

                // check the time spent in the framework
                $this->assertTrue( $profile->getCollector('timer')->getTime() < 0.5);
            }
        }
    }

例えばデータベースのクエリが多すぎたりするなど、データのプロファイリングが理由で、テストが失敗する際は、テスト後にウェブプロファイラを使用しリクエストを分析したいと思われるでしょう。これはエラーメッセージにトークンを埋め込むことで簡単に実現することができます。
::

    $this->assertTrue(
        $profile->get('db')->getQueryCount() < 30,
        sprintf('Checks that query count is less than 30 (token %s)', $profile->getToken())
    );

.. caution::

     プロファイラを格納する場所は、環境によって異なることがあります。(特にデフォルトで使用している SQLite を使用している際には異なります)

.. note::

    クライアントを分離しても、テストに HTTP レイヤーを使用しても、プロファイラの情報は、利用可能です。

.. tip::

    このインタフェースの詳細を学ぶには、ビルトインされたドキュメント :doc:`data collectors</cookbook/profiler/data_collector>` のAPIを参照してください。

.. 2011/11/02 ganchiku 89d8acf9fa096048e2da615ded977a9741aed92b

