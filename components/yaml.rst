YAMLコンポーネント
==================

    YAMLコンポーネントを使うと、yamlを読み込んだり書き出したりすることができます。

それは何?
-----------

Symfony2のYAMLコンポーネントはYAML文字列をパースしてPHPの配列に変換します。
同様に、PHPの配列をYAML文字列に変換することもできます。 

`YAML`_, *YAML Ain't Markup Language* は様々なプログラム言語で利用可能な、人間可読なデータのシリアル化方法です。YAMLは設定ファイルに最適のフォーマットです。 
YAMLはXMLのように様々なデータを表現でき、INIファイルのように読みやすくなっています。 

Symfony2のYAMLコンポーネントは、YAMLのバージョン1.2に対応しています。

インストール
------------

コンポーネントをインストールする方法は何通りもあります。

* 公式Gitレポジトリ  (https://github.com/symfony/Yaml)
* PEARコマンドでインストール ( `pear.symfony.com/Yaml`)
* Composerを使ってインストール (Packagistの `symfony/yaml`)

なぜ?
----

高速
~~~~

SymfonyのYAMLコンポーネントの目的の一つは、処理速度と機能の適正なバランスを見付けることです。設定ファイルを扱うのに必要な機能のみに対応しています。

本格的なパーサー
~~~~~~~~~~~

コンポーネントは、本格的なパーサーを備え、設定ファイルが必要とするようなYAML仕様の大部分に対応しています。
とても堅牢で、わかりやすく、拡張しやすいシンプルな作りをしていることも意味します。

明確なエラーメッセージ
~~~~~~~~~~~~~~~~~~~~

YAMLファイルの書き方に誤りがある時、コンポーネントは問題のあるファイル名と行番号を明示した役に立つエラーメッセージを出力します。
そのため、デバッグが非常に簡単です。

書き出しのサポート
~~~~~~~~~~~~

PHPの配列をオブジェクトも含めて変換し、インラインレベルの設定で簡単にYAMLを書き出すこともできます。

データ型への対応
~~~~~~~~~~~~~

YAMLの組み込みデータ型のほとんどに対応しています。
例えば、日付型、整数型、8進数型、ブール型などです。

キーのマージへの対応
~~~~~~~~~~~~~~~~~~~~~~

参照、エイリアス、キーのマージに完全に対応しています。
共通の設定ファイルを参照し、同じ設定を繰り返し書かなくて良いようにしましょう。

Symfony2のYAMLコンポーネントを使う
---------------------------------

Symfony2のYAMLコンポーネントはとてもシンプルで、2つのクラスで構成されています。
1つはYAML文字列をパースするクラス (:class:`Symfony\\Component\\Yaml\\Parser`)で、もう1つはPHP配列からYAML文字列を書き出すクラス(:class:`Symfony\\Component\\Yaml\\Dumper`)です。

2つのクラスの上位に :class:`Symfony\\Component\\Yaml\\Yaml` クラスがあり、一般的な使い方が簡単にできる薄いラッパーとして動作します。

YAMLファイルを読み込む
~~~~~~~~~~~~~~~~~~

:method:`Symfony\\Component\\Yaml\\Parser::parse` メソッドはYAML文字列をパースしてPHP配列に変換します。:

.. code-block:: php

    use Symfony\Component\Yaml\Parser;

    $yaml = new Parser();

    $value = $yaml->parse(file_get_contents('/path/to/file.yml'));

パース中にエラーが発生した時、パーサーはエラーの種類とエラーが発生したYAML文字列の行番号を示す:class:`Symfony\\Component\\Yaml\\Exception\\ParseException` 例外を投げます。:

.. code-block:: php

    use Symfony\Component\Yaml\Exception\ParseException;

    try {
        $value = $yaml->parse(file_get_contents('/path/to/file.yml'));
    } catch (ParseException $e) {
        printf("Unable to parse the YAML string: %s", $e->getMessage());
    }

.. tip::

    パーサーは再入可能なので、同じパーサーインスタンスを違うYAMLファイルを読み込むのに再利用することができます。

YAMLファイルを読み込む時、:method:`Symfony\\Component\\Yaml\\Yaml::parse` ラッパーメソッドの方がより便利かもしれません。:

.. code-block:: php

    use Symfony\Component\Yaml\Yaml;

    $loader = Yaml::parse('/path/to/file.yml');

:method:`Symfony\\Component\\Yaml\\Yaml::parse` 静的メソッドはYAML文字列またはYAMLを含むファイルを引数に取ります。内部では:method:`Symfony\\Component\\Yaml\\Parser::parse` メソッドを呼びますが、いくつかおまけをつけてくれます。:

* YAMLファイルをPHPファイルとして扱うため、YAMLファイル内にPHPの処理を埋め込むことができます。 

* あるファイルをパースできなかった時、エラーメッセージにファイル名を自動的に付け加えて、複数のYAMLファイルを読み込む時にデバッグしやすくします。

YAMLファイルを書く
~~~~~~~~~~~~~~~~~~

:method:`Symfony\\Component\\Yaml\\Dumper::dump` メソッドはどんなPHP配列でもYAML表現に書き出すことができます。 :

.. code-block:: php

    use Symfony\Component\Yaml\Dumper;

    $array = array('foo' => 'bar', 'bar' => array('foo' => 'bar', 'bar' => 'baz'));

    $dumper = new Dumper();

    $yaml = $dumper->dump($array);

    file_put_contents('/path/to/file.yml', $yaml);

.. note::

    当然のことながら、Symfony2のYAMLダンパーはリソース型を書き出すことはできません。同様に、PHPのオブジェクトを書き出すことができるとしても、サポートされていない機能と考えられます。


書き出し中にエラーが発生した場合、パーサーは:class:`Symfony\\Component\\Yaml\\Exception\\DumpException` 例外を投げます。

1つの配列を書き出すだけなら、ショートカットとして:method:`Symfony\\Component\\Yaml\\Yaml::dump` 静的メソッドを使う事ができます。:

.. code-block:: php

    use Symfony\Component\Yaml\Yaml;

    $yaml = Yaml::dump($array, $inline);

YAMLフォーマットには配列の書き方が2種類あります。拡張形式とインライン形式です。デフォルトでは、ダンパーはインライン形式を使用します。 :

.. code-block:: yaml

    { foo: bar, bar: { foo: bar, bar: baz } }

:method:`Symfony\\Component\\Yaml\\Dumper::dump` メソッドに第二引数を渡すと、配列の書き方を拡張形式からインライン形式に切り替えるレベルを変更することができます。:

.. code-block:: php

    echo $dumper->dump($array, 1);

.. code-block:: yaml

    foo: bar
    bar: { foo: bar, bar: baz }

.. code-block:: php

    echo $dumper->dump($array, 2);

.. code-block:: yaml

    foo: bar
    bar:
        foo: bar
        bar: baz

YAMLフォーマット
---------------

`YAML`_ 公式サイトによれば、YAMLは「人間が読みやすいように最適化された、すべてのプログラミング言語のための標準的なデータシリアライゼーション」です。

複雑なネストのデータ構造も YAML であらわすことができますが、この章では、symfony の設定ファイルを扱うために、YAML について知っておかなければならない最小限の知識を説明します。

YAML はデータを記述するためのシンプルな軽量マークアップ言語です。文字列、ブール値、浮動小数点数、整数のような単純なデータ型をあらわすための構文は PHP と似ています。PHP と異なる構文は配列 (シーケンス) とハッシュ (マッピング) です。

スカラー
~~~~~~~

スカラーの構文は PHP と似ています。

文字列
.......

.. code-block:: yaml

    A string in YAML

.. code-block:: yaml

    'A singled-quoted string in YAML'

.. tip::

    シングルクォートで囲まれた文字列のなかでシングルクォートをあらわすには、シングルクォート( ``'`` )を2つ連ねます。:

    .. code-block:: yaml

        'シングルクォートで囲まれたYAML文字列のなかでのシングルクォート '' '

.. code-block:: yaml

    "ダブルクォートで囲まれたYAMLの文字列\n"

文字列が1つ以上の適切なスペースではじまるもしくは終わる場合には、クォートスタイル (クォートで囲む方法) が適しています。

.. tip::

    ダブルクォートスタイルでは任意の文字列をあらわすのにエスケープシーケンス (``\``) を使うこともできます。こちらのスタイルは文字列に ``\n`` もしくは Unicode を埋め込む場合に適しています。

文字列に改行を入れる場合、パイプ (``|``) によって示されるリテラルスタイルを選ぶことができます。このスタイルでは、文字列は複数行にわたってあらわされ、改行は保たれます。

.. code-block:: yaml

    |
      \/ /| |\/| |
      / / | |  | |__

ほかにも、大なり記号 (``>``) で示される折り畳みスタイルを選ぶことができます。それぞれの改行はスペースに置き換わります。

.. code-block:: yaml

    >
      これはとても長いセンテンスで
      YAML において数行にわたりますが、
      改行コードは追加されず
      文字列としてレンダリングされます。

.. note::

    上記の例では、それぞれの行頭にある2文字分のスペースにご注目ください。これらのスペースは出力結果の PHP 文字列にはあらわれません。

数字
.......

.. code-block:: yaml

    # 整数
    12

.. code-block:: yaml

    # 8進数
    014

.. code-block:: yaml

    # 16進数
    0xC

.. code-block:: yaml

    # 浮動小数点数
    13.4

.. code-block:: yaml

    # 指数
    1.2e+34

.. code-block:: yaml

    # 無限大
    .inf

null
.....

ヌル (ナル) の値は ``null`` もしくはチルダ (``~``) であらわします。

ブール
........

ブール値は ``true`` と ``false`` であらわします。

日付
.....

日付のフォーマットは ISO-8601 標準に準拠します。

.. code-block:: yaml

    2001-12-14t21:59:43.10-05:00

.. code-block:: yaml

    # 単純な日付
    2002-12-14

コレクション
~~~~~~~~~~~

単純なスカラーをあらわすためだけに YAML ファイルを使うことはめったにありません。ほとんどの場合、コレクションをあらわすことになります。コレクションはシーケンスとマッピングのどちらかの要素になります。シーケンスとマッピングは両方とも PHP 配列に変換されます。

シーケンスでは、ダッシュ (``- ``) の直後にスペースを入れます。

.. code-block:: yaml

    - PHP
    - Perl
    - Python

上記の YAML コードは次の PHP コードと同等です。

.. code-block:: php

    array('PHP', 'Perl', 'Python');

マッピングでは、キーと値のペアをあらわすのにコロン (``:``) とスペースを使います。

.. code-block:: yaml

    PHP: 5.2
    MySQL: 5.1
    Apache: 2.2.20

上記の YAML コードは次の PHP コードと同等です。

.. code-block:: php

    array('PHP' => 5.2, 'MySQL' => 5.1, 'Apache' => '2.2.20');

.. note::

    マッピングでは、キーは有効なスカラーの値になります。

少なくともスペースが1つ入っていれば、コロンと値のあいだのスペースの数は問いません。

.. code-block:: yaml

    PHP:    5.2
    MySQL:  5.1
    Apache: 2.2.20

ネストのコレクションをあらわすには、1つもしくは複数のスペースでインデントを入れます。

.. code-block:: yaml

    "symfony 1.0":
      PHP:    5.0
      Propel: 1.2
    "symfony 1.2":
      PHP:    5.2
      Propel: 1.3

上記の YAML コードは次の PHP コードと同等です。

.. code-block:: php

    array(
      'symfony 1.0' => array(
        'PHP'    => 5.0,
        'Propel' => 1.2,
      ),
      'symfony 1.2' => array(
        'PHP'    => 5.2,
        'Propel' => 1.3,
      ),
    );

YAML コードのなかでインデントを入れるときに念頭に置くことが1つあります。 *1つもしくは複数のスペースを使います。タブを使ってはなりません。*.

次のようにシーケンスとマッピングをネストにすることができます。

.. code-block:: yaml

    'Chapter 1':
      - Introduction
      - Event Types
    'Chapter 2':
      - Introduction
      - Helpers

スコープをあらわすのにインデントよりもわかりやすい記号が使われるので、フロースタイルはコレクションをあらわすのに適しています。

シーケンススタイルでは、コレクションは、角かっこ (``[]``) で囲まれ、カンマで区切られたリストとしてあらわされます。

.. code-block:: yaml

    [PHP, Perl, Python]

マッピングスタイルでは、コレクションは、波かっこ (`{}`) で囲まれ、カンマで区切られたキーもしくは値としてあらわされます。

.. code-block:: yaml

    { PHP: 5.2, MySQL: 5.1, Apache: 2.2.20 }

見やすくするために、複数のスタイルを組み合わせることができます。

.. code-block:: yaml

    'Chapter 1': [Introduction, Event Types]
    'Chapter 2': [Introduction, Helpers]

.. code-block:: yaml

    "symfony 1.0": { PHP: 5.0, Propel: 1.2 }
    "symfony 1.2": { PHP: 5.2, Propel: 1.3 }

コメント
~~~~~~~~

文字列の行頭をハッシュ記号 (``#``)にすればコメントになります。

.. code-block:: yaml

    # 行コメント
    "symfony 1.0": { PHP: 5.0, Propel: 1.2 } # Comment at the end of a line
    "symfony 1.2": { PHP: 5.2, Propel: 1.3 }

.. note::

    コメントは YAML パーサーによって無視され、コレクションのネストの現在のレベルにしたがってインデントが入ります。

.. _YAML: http://yaml.org/


.. 2012/01/21 77web f14f7b3d7875b8b97d728fc59a14d22d9034ae8d