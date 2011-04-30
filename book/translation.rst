.. index::
   single: Translations

翻訳
====

「国際化」という言葉は、文字列あるいは他のロケール固有の単位を抽出して
アプリケーションの外部に取り出し、ユーザのロケール (例えば言語や国) に基づいて
翻訳したり変換したりできるレイヤーに落とし込むことです。テキストにおいては、テキスト
(あるいは「メッセージ」) をユーザーの言語に翻訳することのできる機能でラッピング
することを意味します。

::

    // テキストは *常に* 英語で表示されます
    echo 'Hello World';

    // テキストはエンドユーザーの言語に翻訳されるか、
    // デフォルトの英語になります
    echo $translator->trans('Hello World');

.. note::

    *ロケール* という言葉は、大雑把に言うとユーザーの言語や国のことです。
    アプリケーションが後で翻訳や他のフォーマットの違い (例えば通貨単位)
    を管理するのに使うことができます。 ISO639-1 *言語* コード、アンダースコア
    (``_``) 、そして ISO3166 *国* コード (例えば ``fr_FR`` がフランス語と国の
    フランスを意味します) を使うことをおすすめします。

この章では、アプリケーションが複数のロケールをサポートするための準備方法について学び、
その後、複数のロケールに対する翻訳の作成方法を学びます。全体として、このプロセスには
いくつかの一般的な手順があります。

1. Symfony の ``Translation`` コンポーネントを有効化し、設定します。
2. ``Translator`` の呼び出しの中で文字列 (例えば「メッセージ」)をラッピングして
   抽出します。
3. アプリケーション内で各メッセージを翻訳する、サポートされるロケールごとの翻訳
   リソースを作ります。
4. セッション内でユーザーのロケールを判断し、設定し、管理します。

.. index::
   single: Translations; Configuration

設定
----

翻訳は、翻訳されたメッセージを探して返すためにユーザーのロケールを使用する、\ ``Translator`` :term:`サービス` が担当します。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            translator: { fallback: en }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:translator fallback="en" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'translator' => array('fallback' => 'en'),
        ));

``fallback`` オプションは、ユーザーのロケール内に翻訳が含まれなかった時の
フォールバックロケールを定義しています。

.. tip::

    ロケールに対する翻訳が存在しない時、トランスレータはまず言語
    (インスタンスに対するロケールが ``fr_FR`` の時は ``fr``) を探します。
    それでも失敗する場合、フォールバックロケールを使った訳を探します。

翻訳で使われるロケールは、ユーザセッション内に保存されているものです。

.. index::
   single: Translations; Basic translation

標準的な翻訳
------------

テキストの翻訳は、\ ``translator`` サービス (:class:`Symfony\\Component\\Translation\\Translator`)
を通じて行われます。テキストのブロック (*メッセージ* と呼びます) を翻訳するには、
:method:`Symfony\\Component\\Translation\\Translator::trans` メソッドを使用してください。
例として、コントローラの中から単純なメッセージを翻訳していると考えてください。:

.. code-block:: php

    public function indexAction()
    {
        $t = $this->get('translator')->trans('Symfony2 is great');

        return new Response($t);
    }

このコードが実行されると、 Symfony2 はユーザーの ``locale`` に基づいた
"Symfony2 is great" というメッセージを翻訳しようとします。この動作のために、
与えられたロケールで翻訳されたメッセージの集まりである「翻訳リソース」を通じて
どのようにメッセージを翻訳するのかを Symfony2 に教える必要があります。翻訳の
「辞書」は幾つかの異なるフォーマットで作られる必要があります。 XML が推奨される
フォーマットです。

.. configuration-block::

    .. code-block:: xml

        <!-- messages.fr.xml -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Symfony2 is great</source>
                        <target>J'aime Symfony2</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // messages.fr.php
        return array(
            'Symfony2 is great' => 'J\'aime Symfony2',
        );

    .. code-block:: yaml

        # messages.fr.yml
        Symfony2 is great: J'aime Symfony2

ユーザーのロケールがフランス語 (例えば ``fr_FR`` または ``fr_BE``) の時には、
メッセージは ``J'aime Symfony2`` に翻訳されます。

翻訳のプロセス
~~~~~~~~~~~~~~

実際にメッセージを翻訳するには、 Symfony2 はシンプルなプロセスで行います。

* セッションに保存されているユーザーの ``locale`` を見つけ出します。

* 翻訳済みメッセージのカタログが ``ロケール`` (例えば ``fr_FR``) に定義されている
  翻訳リソースからロードされます。フォールバックロケールからのメッセージも
  同じようにロードされ、まだ存在していない場合にはカタログに追加されます。最終的な
  結果は、翻訳の大きな「辞書」になります。詳しくは `メッセージのカタログ`_ を参照してください。

* メッセージがカタログの中にある場合、翻訳結果が戻り値になります。カタログの中にない場合、
  トランスレータは元のメッセージを返します。

``trans()`` メソッドを使用する時は、 Symfony2 は適切なメッセージカタログの中から
一致する文字列を探し、その文字列を返します (メッセージが存在する場合)。

.. index::
   single: Translations; Message placeholders

メッセージプレースホルダー
~~~~~~~~~~~~~~~~~~~~~~~~~~

時によって、メッセージは翻訳の必要がある変数を含んでいることがあります。

.. code-block:: php

    public function indexAction($name)
    {
        $t = $this->get('translator')->trans('Hello '.$name);

        return new Response($t);
    }

ところが、トランスレータは変数部分 (例えば "Hello Ryan" や "Hello Fabien")
を含んだ完全に一致するメッセージを探そうとするので、このような文字列に対する
翻訳を行うのは無理です。 ``$name`` 変数の考えうるすべてのイテレーションに対して
訳をつける代わりに、変数を「プレースホルダー」で置き換えることができます。

.. code-block:: php

    public function indexAction($name)
    {
        $t = $this->get('translator')->trans('Hello %name%', array('%name%' => $name));

        new Response($t);
    }

これで、 Symfony2 はそのままのメッセージ (``Hello %name%``) の翻訳を探すようになります。
そして *その後で* プレースホルダーを変数の値に置き換えます。翻訳の生成は前と同じように行われます。

.. configuration-block::

    .. code-block:: xml

        <!-- messages.fr.xml -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Hello %name%</source>
                        <target>Bonjour %name%</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // messages.fr.php
        return array(
            'Hello %name%' => 'Bonjour %name%',
        );

    .. code-block:: yaml

        # messages.fr.yml
        'Hello %name%': Hello %name%

.. note::

    全体のメッセージが PHP の `strtr 関数`_ で再構築されるように、プレースホルダーは
    どのようなかたちをとることもできます。しかし、 Twig テンプレート内で翻訳を行う時は、
    ``%var%`` 表記が必須になるので、全体として従うに値する規約といえます。

ここまで見てきたように、翻訳を作成するには2つのステップがあります。

1. ``Translator`` を通じて処理を行うことによって、翻訳に必要なメッセージを
   抽出します。
2. サポートしたいロケールごとにメッセージの翻訳を作成します。

次のステップは異なるロケールに対する翻訳を定義したメッセージのカタログの作成です。

.. index::
   single: Translations; Message catalogues

メッセージのカタログ
--------------------

メッセージが翻訳された時、 Symfony2 はユーザーのロケールに対するメッセージの
カタログをコンパイルし、メッセージの翻訳を探します。メッセージのカタログは、
特定のロケールに対する翻訳の辞書のようなものです。例えば、\ ``fr_FR`` ロケールに
対するカタログは、以下のような訳を含んでいます。

    Symfony2 is Great => J'aime Symfony2

これらの訳を作るのは、国際化されたアプリケーションの開発者 (または翻訳者)
の責任です。翻訳はファイルシステム上に保存され、いくつかの規約の結果、
Symfony に発見されます。

.. index::
   single: Translations; Translation resource locations

翻訳の場所と名前付け規約
~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 はメッセージファイル (例として翻訳) を2つの場所から探します。

* バンドル内で見つけたメッセージに対しては、対応するメッセージファイルは
  バンドルの ``Resources/translations/`` ディレクトリに存在する必要があります。

* バンドルの翻訳をオーバーライドするには、メッセージファイルを ``app/translations``
  に置いてください。

Symfony2 が翻訳の詳細を理解するのに規約を使用するので、翻訳のファイルネームも重要です。
それぞれのメッセージファイルは、\ ``ドメイン.ロケール.ローダー`` というパターンに沿って
いなければなりません。

* **ドメイン**: メッセージをグループに体系づける任意の方法です (例えば ``admin``\ 、
  ``navigation`` またはデフォルトの ``messages``)。詳しくは `メッセージドメインの使用`_
  を参照してください。

* **ロケール**: その翻訳のロケールです (例えば ``en_GB`` や ``en`` など)。

* **ローダー**: Symfony2 がどのようにファイルをロードし、パースするかです (例えば
  ``xml`` や ``php``\ 、\ ``yml``)。

ローダーは、あらゆる登録済みのローダーの名前になり得ます。デフォルトでは、
Symfony は以下のローダーを提供しています。

* ``xml``: XLIFF ファイル
* ``php``:   PHP ファイル
* ``yml``:  YAML ファイル

どのローダーを使用するかは完全にあなた (開発者) 次第で、好みの問題です。

.. note::

    翻訳はデータベースや、 :class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface`
    の実装であるカスタムクラスによって定義されるその他のストレージに保存することも
    できます。どのようにカスタムローダーを登録するかは
    :doc:`Custom Translation Loaders </cookbook/translation/custom_loader>`
    を参照してください (訳注 : 2011/03/13現在、この項は存在していない模様)。

.. index::
   single: Translations; Creating translation resources

翻訳の作成
~~~~~~~~~~

それぞれのファイルは、与えられたドメインとロケールに対する ID と翻訳のペアの連なりから
できています。この ID はそれぞれの翻訳の識別子になっており、アプリケーションあるいは
ユニークな識別子 (例えば "symfony2.great" といったものです。詳しくはこの後の補足を
参照してください) のメインロケールのメッセージを引くことができます。

.. configuration-block::

    .. code-block:: xml

        <!-- src/Sensio/MyBundle/Resources/translations/messages.fr.xml -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Symfony2 is great</source>
                        <target>J'aime Symfony2</target>
                    </trans-unit>
                    <trans-unit id="2">
                        <source>symfony2.great</source>
                        <target>J'aime Symfony2</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // src/Sensio/MyBundle/Resources/translations/messages.fr.php
        return array(
            'Symfony2 is great' => 'J\'aime Symfony2',
            'symfony2.great'    => 'J\'aime Symfony2',
        );

    .. code-block:: yaml

        # src/Sensio/MyBundle/Resources/translations/messages.fr.yml
        Symfony2 is great: J'aime Symfony2
        symfony2.great:    J'aime Symfony2

Symfony2 はこれらのファイルを見つけ出し、\ "Symfony2 is great" や "symfony2.great"
の両方をフランス語ロケール (``fr_FR`` や ``fr_BE``) に翻訳するのに使います。

.. sidebar:: 実際のメッセージあるいはキーワードを使う

    この例では、翻訳されるメッセージを作る時の2つの異なる哲学を表しています。

    .. code-block:: php

        $t = $translator->trans('Symfony2 is great');

        $t = $translator->trans('symfony2.great');

    最初の方法では、メッセージはデフォルトロケールで書かれています
    (この場合英語) 。このメッセージは、翻訳を作る際に "id" として使用されます。

    2番目の方法では、メッセージは実際にはメッセージの意味を伝える「キーワード」に
    なっています。キーワードメッセージはそれからそれぞれの翻訳の「ID」として
    使われます。この場合、翻訳はデフォルトロケール用に作られる必要があります
    (例えば ``symfony2.great`` は ``Symfony2 is great`` に訳される)。

    デフォルトロケールのメッセージを "Symfony2 is really great" にしたいと考えた
    場合でも、それぞれの翻訳ファイル内のメッセージキーを変更する必要がないことから、
    2番目の方法は便利です。

    どちらの方法を使うかは完全にあなた次第ですが、「キーワード」フォーマットは
    常に推奨される方法です。

    それに加えて、\ ``php`` と ``yaml`` ファイルフォーマットは、 ID に対して
    キーワードの代わりに実際のテキストを使用する時に同じ ID が繰り返されるのを
    防ぐため、ネストされた ID をサポートしています。

    .. configuration-block::

        .. code-block:: yaml

            symfony2:
                is:
                    great: Symfony2 is great
                    amazing: Symfony2 is amazing
                has:
                    bundles: Symfony2 has bundles
            user:
                login: Login

        .. code-block:: php

            return array(
                'symfony2' => array(
                    'is' => array(
                        'great' => 'Symfony2 is great',
                        'amazing' => 'Symfony2 is amazing',
                    ),
                    'has' => array(
                        'bundles' => 'Symfony2 has bundles',
                    ),
                ),
                'user' => array(
                    'login' => 'Login',
                ),
            );

    複数階層はレベルごとにドット (.) で区切られてひとつの ID と翻訳のペアに
    なります。従って、上の例は下のコードと同じ意味になります。

    .. configuration-block::

        .. code-block:: yaml

            symfony2.is.great: Symfony2 is great
            symfony2.is.amazing: Symfony2 is amazing
            symfony2.has.bundles: Symfony2 has bundles
            user.login: Login

        .. code-block:: php

            return array(
                'symfony2.is.great' => 'Symfony2 is great',
                'symfony2.is.amazing' => 'Symfony2 is amazing',
                'symfony2.has.bundles' => 'Symfony2 has bundles',
                'user.login' => 'Login',
            );

.. index::
   single: Translations; Message domains

メッセージドメインの使用
------------------------

これまで見てきたように、メッセージファイルは翻訳されたロケールごとにまとめられます。
また、さらに「ドメイン」ごとにもまとめることができます。メッセージファイルを作成
するさい、ドメインはファイル名の最初の部分になります。デフォルトのドメインは
``messages`` です。例えば、管理上、翻訳が ``messages`` と ``admin`` と ``navigation``
という 3 つのドメインに分けられていると考えてください。フランス語の翻訳は以下の
メッセージファイルになります。

* ``messages.fr.xml``
* ``admin.fr.xml``
* ``navigation.fr.xml``

デフォルトドメイン (``messages``) 内に翻訳文字列がない時には、 ``trans()`` の
3 番目の引数としてドメイン名を指定する必要があります。

.. code-block:: php

    $this->get('translator')->trans('Symfony2 is great', array(), 'admin');

Symfony2 はここでユーザーのロケールの ``admin`` ドメイン内のメッセージを探します。

.. index::
   single: Translations; User's locale

ユーザーロケールの扱い
----------------------

現在のユーザのロケールはセッションに保存され、\ ``session`` サービスを介して
アクセスできます。

.. code-block:: php

    $locale = $this->get('session')->getLocale();

    $this->get('session')->setLocale('en_US');

.. index::
   single: Translations; Fallback and default locale

フォールバックロケールとデフォルトロケール
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

セッション内でロケールが明確に指定されていない場合、\ ``fallback_locale`` 設定パラメータが
``Translator`` で使用されます。このパラメータのデフォルトは ``en`` です
(詳しくは `設定`_ を参照してください) 。

もう一つの方法として、セッションサービスに ``default_locale`` を定義することで、
ユーザーのセッションにロケールが設定されていることを保証できます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session: { default_locale: en }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:session default-locale="en" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'session' => array('default_locale' => 'en'),
        ));

ロケールと URL
~~~~~~~~~~~~~~

ユーザーのロケールはセッション内に保存されるので、ユーザーのロケールに基づいた
色々な言語のリソースを表示するのに、同じ URL が使われることになります。
例えば、\ ``http://www.example.com/contact`` はあるユーザーには英語で、別なユーザーには
フランス語で表示されます。残念ながら、これは Web の基本的なルール、すなわち、
ある URL はユーザーに関係なく同じリソースを返す、というルールに反しています。
さらに問題がややこしくなるのが、どのバージョンのコンテンツが検索エンジンでインデックス
されるのか？ということです。

望ましいやり方は、URL にロケールを含めることです。これは、特別な ``_locale``
パラメータを使ったルーティングシステムで、完全にサポートされています。

.. configuration-block::

    .. code-block:: yaml

        contact:
            pattern:   /{_locale}/contact
            defaults:  { _controller: MyContactBundle:Contact:index, _locale: en }
            requirements:
                _locale: en|fr|de

    .. code-block:: xml

        <route id="contact" pattern="/{_locale}/contact">
            <default key="_controller">MyContactBundle:Contact:index</default>
            <default key="_locale">en</default>
            <requirement key="_locale">en|fr|de</requirement>
        </route>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/{_locale}/contact', array(
            '_controller' => 'MyContactBundle:Contact:index',
            '_locale'     => 'en',
        ), array(
            '_locale'     => 'en|fr|de'
        )));
        $collection->addCollection($loader->import("HelloBundle/Resources/config/routing.php"));

        return $collection;

ルートの中で特別な `_locale` パラメータを使用する際、一致するロケールが
*自動的にユーザーセッションに設定されます* 。言い換えると、ユーザーが
``/fr/contact`` という URI を訪れると、\ ``fr`` というロケールが自動的に
ユーザーのセッションのロケールとして設定されます。

これで、ユーザーのロケールをアプリケーション内の他の翻訳されたページへのルートを
作るのに使用できるようになります。

.. index::
   single: Translations; Pluralization

複数型への対応
--------------

メッセージの複数型への対応は、ルールがなかなか複雑であるため、大きな問題です。
例えば、これはロシア語の複数型の数学的表現です。

::

    (($number % 10 == 1) && ($number % 100 != 11)) ? 0 : ((($number % 10 >= 2) && ($number % 10 <= 4) && (($number % 100 < 10) || ($number % 100 >= 20))) ? 1 : 2);

見ての通り、ロシア語では、それぞれ 0、1 あるいは 2 のインデックスを与えられた 3 つの異なる
複数型の表現があります。それぞれの表現で複数形は異なりますので、翻訳も同様に異なります。

複数型への対応のために翻訳の表現が異なる時、それら全ての表現をパイプ (``|``) で
区切られた文字列として与えることができます。

::

    'There is one apple|There are %count% apples'

複数型に対応したメッセージを翻訳するため、 :method:`Symfony\\Component\\Translation\\Translator::transChoice` メソッドを使用できます。

.. code-block:: php

    $t = $this->get('translator')->transChoice(
        'There is one apple|There are %count% apples',
        10,
        array('%count%' => 10)
    );

2つ目の引数 (この例では ``10``) は記述されるオブジェクトの *数* であり、
どの翻訳が使われるかを決めるのに使われ、\ ``%count%`` プレースホルダーに
投入されます。

与えられた数字に従い、トランスレータは適切な複数型の表現を選びます。
英語の場合、多くの単語はぴったり1つしかオブジェクトがない時には
単数形で、それ以外の数 (0, 2, 3...) の時には複数型になります。
従って、\ ``count`` が ``1`` の時には、トランスレータ―は最初の文字列
(``There is one apple``) を翻訳として使い、そうでない場合は
``There are %count% apples`` を使用します。

以下はフランス語の翻訳です。

::

    'Il y a %count% pomme|Il y a %count% pommes'

文字列は同じように見えたとしても (パイプで区切られた2つの部分文字列からなっています)、
フランス語の表現は異なります。最初の表現 (複数形ではない) は ``count`` が ``0`` か
``1`` の時に使われます。従って、トランスレータは ``count`` が ``0`` または ``1``
の時には自動的に最初の文字列 (``Il y a %count% pomme``) を使います。

それぞれのロケールは独自の表現のセットを持っています。いくつかのロケールは、どの数字が
どの複数形にマップされるかの複雑なルールがある、6つの異なる複数形の表現を持っています。
英語とフランス語のルールはかなりシンプルですが、ロシア語では、どの表現がどの文字列に
一致するのか知るためにヒントが欲しくなるでしょう。翻訳者を手助けするために、
オプションとしてそれぞれの文字列に対して「タグをつける」ことができます。

::

    'one: There is one apple|some: There are %count% apples'

    'none_or_one: Il y a %count% pomme|some: Il y a %count% pommes'

タグは翻訳者のためのヒントでしかありませんので、どの複数形の表現を使うか決める
ロジックには影響しません。タグはコロン (``:``) で終わる説明を含む文字列になります。
また、タグが翻訳された元のメッセージと同じである必要はありません。

.. tip:

    タグはオプションですので、トランスレータはタグを使用しません (トランスレータは
    単純に文字列内の位置に応じた文字列を取得するだけです) 。

間隔を明示した複数型への対応
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

メッセージを複数形にする最も簡単な方法は、与えられた数を元にどの文字列を選ぶかの
Symfony2 の内部ロジックを使うことです。場合によっては、もっと翻訳の動作を自由に
制御したかったり、違う訳が欲しい時があるでしょう (例えば ``0`` に関してや、負の数の時)。
このような場合、明示的な数の間隔を使用できます。

::

    '{0} There is no apples|{1} There is one apple|]1,19] There are %count% apples|[20,Inf] There are many apples'

間隔は `ISO 31-11`_ 規格に従っています。上の文字列は 4 つの異なる間隔を定義しています。
ちょうど ``0``\ 、ちょうど ``1``\ 、\ ``2 から 19``\ 、\ ``20`` 以上、です。

明示的な数の表現と、標準の表現を混在することもできます。この場合、数が指定された
間隔と一致しない時には、明示的な表現が削除された後に標準の表現が有効になります。

::

    '{0} There is no apples|[20,Inf] There are many apples|There is one apple|a_few: There are %count% apples'

例えば、\ ``1`` つのリンゴの場合、標準の表現 ``There is one apple`` が使われます。
``2から19`` 個のリンゴの場合、2番目の標準の表現である ``There are %count%
apples`` が使われます。

:class:`Symfony\\Component\\Translation\\Interval` クラスで数の有限集合を表せます。

::

    {1,2,3,4}

または 2 と他の数の間なら以下のようになります。

::

    [1, +Inf[
    ]-1,2[

左側のデリミターは ``[`` (含む) または ``]`` (含まない) になります。
右側のデリミターは ``[`` (含まない) または ``]`` (含む) です。
数と合わせて、\ ``-Inf`` と ``+Inf`` を無限を表すのに使用できます。

.. index::
   single: Translations; In templates

テンプレート内の翻訳
--------------------

多くの場合、翻訳はテンプレート内で発生します。 Symfony2 は Twig と PHP テンプレートの
両方をネイティブでサポートします。

Twig テンプレート
~~~~~~~~~~~~~~~~~

Symfony2 はメッセージの翻訳に役立つよう特別な Twig タグ (``trans`` と ``transChoice``)
を提供します。

.. code-block:: jinja

    {{ "Symfony2 is great" | trans }}

    {% trans "Symfony2 is great" %}

    {% trans %}
        Foo %name%
    {% endtrans %}

    {% transchoice count %}
        {0} There is no apples|{1} There is one apple|]1,Inf] There are %count% apples
    {% endtranschoice %}

``transChoice`` タグは自動的に現在のコンテキストから ``%count%`` 変数を取り出し、
トランスレータに渡します。このメカニズムは ``%var%`` というパターンに従った
プレースホルダーを使用した場合にのみ動作します。

メッセージドメインも指定することができます。

.. code-block:: jinja

    {{ "Symfony2 is great" | trans([], "app") }}

    {% trans "Symfony2 is great" from "app" %}

    {% trans from "app" %}
        Foo %name%
    {% endtrans %}

    {% transchoice count from "app" %}
        {0} There is no apples|{1} There is one apple|]1,Inf] There are %count% apples
    {% endtranschoice %}

PHP テンプレート
~~~~~~~~~~~~~~~~

トランスレータサービスへは、\ ``translator`` ヘルパーを通じて PHP テンプレートからも
アクセスできます。

.. code-block:: html+php

    <?php echo $view['translator']->trans('Symfony2 is great') ?>

    <?php echo $view['translator']->transChoice(
        '{0} There is no apples|{1} There is one apple|]1,Inf[ There are %count% apples',
        10,
        array('%count%' => 10)
    ) ?>

翻訳ロケールの強制
------------------

メッセージの翻訳の際、 Symfony2 はユーザーセッションからのロケール、あるいは
必要な場合は ``フォールバック`` ロケールを使用します。同様に、翻訳で使用する
ロケールを手動で指定することもできます。

.. code-block:: php

    $this->get('translation')->trans(
        'Symfony2 is great',
        array(),
        'messages',
        'fr_FR',
    );

    $this->get('translation')->trans(
        '{0} There is no apples|{1} There is one apple|]1,Inf[ There are %count% apples',
        10,
        array('%count%' => 10),
        'messages',
        'fr_FR',
    );

データベースコンテンツの翻訳
----------------------------

データベースコンテンツの翻訳は `Translatable Extension`_ を通じて Doctrine
によって扱われるべきです。詳しくは、ライブラリのドキュメントを参照してください。

まとめ
------

Symfony2 の Translation コンポーネントを使用すると、国際化されたアプリケーションを
作ることはもはや苦痛なプロセスではなく、いくつかの基本的なステップに要約する
ことができます。

* :method:`Symfony\\Component\\Translation\\Translator::trans` メソッドまたは
  :method:`Symfony\\Component\\Translation\\Translator::transChoice` メソッドの
  いずれかでラッピングすることによって、アプリケーション内のメッセージを抽象化します。

* それぞれのメッセージを、翻訳メッセージファイルを作成することで複数のロケールに
  翻訳します。 メッセージファイルの名前は指定された規約に則っており、Symfony2 は
  それぞれのファイルを見つけ出して処理を行います。

* セッションに保存されているユーザーのロケールを管理します。

.. _`strtr 関数`: http://www.php.net/manual/en/function.strtr.php
.. _`ISO 31-11`: http://en.wikipedia.org/wiki/Interval_%28mathematics%29#The_ISO_notation
.. _`Translatable Extension`: https://github.com/l3pp4rd/DoctrineExtensions
