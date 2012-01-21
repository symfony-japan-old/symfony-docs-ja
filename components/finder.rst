.. index::
   single: Finder

Finderコンポーネント
====================

   Finderコンポーネントは、ファイルやディレクトリを速く簡単に見つけてくれます。

インストール
------------

コンポーネントをインストールする方法は何通りもあります。

* 公式Gitレポジトリ (https://github.com/symfony/Finder);
* PEARコマンドでインストール ( `pear.symfony.com/Finder`);
* Composerを使ってインストール (Packagistの `symfony/finder` ).

使用方法
--------

:class:`Symfony\\Component\\Finder\\Finder` クラスは、ファイルやディレクトリ、またはその両方を見つけます。::

    use Symfony\Component\Finder\Finder;

    $finder = new Finder();
    $finder->files()->in(__DIR__);

    foreach ($finder as $file) {
        print $file->getRealpath()."\n";
    }

``$file`` は :class:`Symfony\\Component\\Finder\\SplFileInfo` のインスタンスです。
:class:`Symfony\\Component\\Finder\\SplFileInfo` は :phpclass:`SplFileInfo` を拡張して相対パスを扱うメソッドを提供するものです。

上記のコードでは、再帰的に現在のディレクトリ内の全てのファイルの名前を出力します。 Finder クラスは、綺麗なインタフェースを使用しており、全てのメソッドは、 Finder のインスタンスを返します。

.. tip::

    Finder インスタンスは、PHPの `Iterator`_ です。つまり、Finder をイテレーションする際に ``foreach`` を使用せずに、 :phpfunction:`iterator_to_array` メソッドを使って配列に変換できますし、 :phpfunction:`iterator_count` を使用して要素を数えることもできます。

条件
----

位置(Location)
~~~~~~~~~~~~~~

唯一の強制的な条件は、位置を指定することです。これは、 Finder にどのディレクトリを検索するのか伝えるのに必要です。::

    $finder->in(__DIR__);

    $finder->in(__DIR__);

複数の位置を検索するには、 :method:`Symfony\\Component\\Finder\\Finder::in` メソッドをつなげてください。::

    $finder->files()->in(__DIR__)->in('/elsewhere');

特定のディレクトリを検索対象から外すには、 :method:`Symfony\\Component\\Finder\\Finder::exclude` メソッドを使用してください。::

    $finder->in(__DIR__)->exclude('ruby');

Finder は PHP のイテレータを使用しているので、 `protocol`_ をサポートするどんな URL も渡すことができます。::

    $finder->in('ftp://example.com/pub/');

もちろん、ユーザ定義のストリームも使用することができます。::

    use Symfony\Component\Finder\Finder;

    $s3 = new \Zend_Service_Amazon_S3($key, $secret);
    $s3->registerStreamWrapper("s3");

    $finder = new Finder();
    $finder->name('photos*')->size('< 100K')->date('since 1 hour ago');
    foreach ($finder->in('s3://bucket-name') as $file) {
        // do something

        print $file->getFilename()."\n";
    }

.. note::

    自分自身でストリームを作成する際には、 `Streams`_ のドキュメントを参照してください。

ファイルもしくはディレクトリ
~~~~~~~~~~~~~~~~~~~~~~~~~~~~


デフォルトでは、 Finder はファイルとディレクトリを返しますが、以下の指定の際は異なります。
:method:`Symfony\\Component\\Finder\\Finder::files`
:method:`Symfony\\Component\\Finder\\Finder::directories` methods control that::

    $finder->files();

    $finder->directories();

リンクをフォローしたい際には、 ``followLinks()`` メソッドを使用してください。::

    $finder->files()->followLinks();

デフォルトでは、イテレータは VCS ファイルを無視します。 ``ignoreVCS()`` メソッドを使うと、無視しないようにできます。::

    $finder->ignoreVCS(false);

ソート
~~~~~~

名前や種類(ディレクトリが先で、次にファイル)によるソート::

    $finder->sortByName();

    $finder->sortByType();

.. note::

    ``sort*`` メソッドは、ソートをする際に、全ての要素が必要です。もちろん大きなイテレータにおいては、遅くなります。

``sort()`` メソッドを使用し、自分自身でソートのアルゴリズムを定義することもできます。::

    $sort = function (\SplFileInfo $a, \SplFileInfo $b)
    {
        return strcmp($a->getRealpath(), $b->getRealpath());
    };

    $finder->sort($sort);

ファイル名
~~~~~~~~~~

:method:`Symfony\\Component\\Finder\\Finder::name` メソッドを使用すれば、名前による絞り込みができます。::

    $finder->files()->name('*.php');

``name()`` メソッドは、 グロブ、文字列、正規表現をサポートしています::

    $finder->files()->name('/\.php$/');

``notNames()`` メソッドは、パターンにマッチしたファイルを除外します。:

    $finder->files()->notName('*.rb');

ファイルサイズ
~~~~~~~~~~~~~~

:method:`Symfony\\Component\\Finder\\Finder::size` メソッドを使用すれば、サイズによる絞り込みができます。::

    $finder->files()->size('< 1.5K');

``size()`` メソッドをつなげて呼ぶことによって、範囲の絞り込みができます。::

    $finder->files()->size('>= 1K')->size('<= 2K');

条件のオペレータは、次のものが使用可能です: ``>``, ``>=``, ``<``, '<=', '==' 。

対象となる値には、キロバイト(``k``, ``ki``)、メガバイト(``m``, ``mi``)、ギガバイト(``g``, ``gi``)といった大きさを使用することができます。接尾辞の ``i`` があると、 `IEC standard`_ に一致している適切な ``2xxn`` バージョンを使用します。


ファイルの日付
~~~~~~~~~~~~~~

:method:`Symfony\\Component\\Finder\\Finder::date` メソッドを使用すれば、ファイルの更新日時による絞り込みができます。::

    $finder->date('since yesterday');

条件オペレータは、次のものが使用できます: ``>``, ``>=``, ``<``, '<=','==' 。 また、 ``since`` や ``after`` を、 ``>`` のエイリアスとして使用できます。同様に、 ``until`` や ``before`` を、 ``<`` のエイリアスとして使用できます。

対象とする値は、 `sttotime`_ 関数によってサポートされている日付なら大丈夫です。


ディレクトリの深さ
~~~~~~~~~~~~~~~~~~

デフォルトでは、 Finder はディレクトリを再帰的に調べます。 :method:`Symfony\\Component\\Finder\\Finder::depth` を使用すれば、調べる深さを制限することができます。::

    $finder->depth('== 0');
    $finder->depth('< 3');

カスタムフィルター
~~~~~~~~~~~~~~~~~~

:method:`Symfony\\Component\\Finder\\Finder::filter` メソッドを使用すれば、オリジナルの戦略でファイルの絞り込みができます。::

    $filter = function (\SplFileInfo $file)
    {
        if (strlen($file) > 10) {
            return false;
        }
    };

    $finder->files()->filter($filter);

``filter()`` メソッドは、引数としてクロージャを受け取ります。マッチしたファイルは、 :phpclass:`SplFileInfo` のインスタンスとして扱うことができます。クロージャが ``false`` を返すと、 そのファイルは検索結果から除外されます。

.. _strtotime:   http://www.php.net/manual/en/datetime.formats.php
.. _Iterator:     http://www.php.net/manual/en/spl.iterators.php
.. _protocol:     http://www.php.net/manual/en/wrappers.php
.. _Streams:      http://www.php.net/streams
.. _IEC standard: http://physics.nist.gov/cuu/Units/binary.html


.. 2012/01/21 77web 43cdd28ff3970576ae63c59e3e43649901ed50df