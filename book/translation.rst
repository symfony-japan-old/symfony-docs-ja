.. index::
   single: Translations

.. Translations
   ============

翻訳
====

.. The term "internationalization" refers to the process of abstracting strings
   and other locale-specific pieces out of your application and into a layer
   where they can be translated and converted based on the user's locale (i.e.
   language and country). For text, this means wrapping each with a function
   capable of translating the text (or "message") into the language of the user::

「国際化」という言葉は、文字列あるいは他のロケール固有の単位を抽出して
アプリケーションの外部に取り出し、ユーザのロケール (例えば言語や国) に基づいて
翻訳したり変換したりできるレイヤーに落とし込むことです。テキストにおいては、テキスト
(あるいは「メッセージ」) をユーザーの言語に翻訳することのできる機能でラッピング
することを意味します。::

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

..  The term *locale* refers roughly to the user's language and country. It
    can be any string that your application then uses to manage translations
    and other format differences (e.g. currency format). We recommended the
    ISO639-1 *language* code, an underscore (``_``), then the ISO3166 *country*
    code (e.g. ``fr_FR`` for French/France).

.. In this chapter, we'll learn how to prepare an application to support multiple
   locales and then how to create translations for multiple locales. Overall,
   the process has several common steps:

この章では、アプリケーションが複数のロケールをサポートするための準備方法について学び、
その後、複数のロケールに対する翻訳の作成方法を学びます。全体として、このプロセスには
いくつかの一般的な手順があります。

.. 1. Enable and configure Symfony's ``Translation`` component;
.. 1. Abstract strings (i.e. "messages") by wrapping them in calls to the ``Translator``;
.. 1. Create translation resources for each supported locale that translate
   each message in the application;
.. 1. Determine, set and manage the user's locale in the session.

1. Symfony の ``Translation`` コンポーネントを有効化し、設定します。

1. ``Translator`` の呼び出しの中で文字列 (例えば「メッセージ」)をラッピングして
   抽出します。

1. アプリケーション内で各メッセージを翻訳する、サポートされるロケールごとの翻訳
   リソースを作ります。

1. セッション内でユーザーのロケールを判断し、設定し、管理します。

.. index::
   single: Translations; Configuration

.. Configuration
   -------------

設定
----

.. Translations are handled by a ``Translator`` :term:`service` that uses the
   user's locale to lookup and return translated messages. Before using it,
   enable the ``Translator`` in your configuration:

翻訳は、翻訳されたメッセージを探して返すためにユーザーのロケールを使用する、
 ``Translator`` :term:`サービス` が担当します。

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

.. The ``fallback`` option defines the fallback locale when a translation does
   not exist in the user's locale.

``fallback`` オプションは、ユーザーのロケール内に翻訳が含まれなかった時の
フォールバックロケールを定義しています。

.. tip::

    ロケールに対する翻訳が存在しない時、トランスレータはまず言語
    (インスタンスに対するロケールが ``fr_FR`` の時は ``fr``) を探します。
    それでも失敗する場合、フォールバックロケールを使った訳を探します。

..    When a translation does not exist for a locale, the translator first tries
      to find the translation for the language (``fr`` if the locale is
      ``fr_FR`` for instance). If this also fails, it looks for a translation
      using the fallback locale.

.. The locale used in translations is the one stored in the user session.

翻訳で使われるロケールは、ユーザセッション内に保存されているものです。

.. index::
   single: Translations; Basic translation

.. Basic Translation
   -----------------

標準的な翻訳
------------

.. Translation of text is done through the  ``translator`` service
   (:class:`Symfony\\Component\\Translation\\Translator`). To translate a block
   of text (called a *message*), use the
   :method:`Symfony\\Component\\Translation\\Translator::trans` method. Suppose,
   for example, that we're translating a simple message from inside a controller:

テキストの翻訳は、``translator`` サービス (:class:`Symfony\\Component\\Translation\\Translator`) 
を通じて行われます。テキストのブロック (*メッセージ* と呼びます) を翻訳するには、
:method:`Symfony\\Component\\Translation\\Translator::trans` メソッドを使用してください。
例として、コントローラの中から単純なメッセージを翻訳していると考えてください。:

.. code-block:: php

    public function indexAction()
    {
        $t = $this->get('translator')->trans('Symfony2 is great');

        return new Response($t);
    }

.. When this code is executed, Symfony2 will attempt to translate the message
   "Symfony2 is great" based on the ``locale`` of the user. For this to work,
   we need to tell Symfony2 how to translate the message via a "translation
   resource", which is a collection of message translations for a given locale.
   This "dictionary" of translations can be created in several different formats,
   XML being the recommended format:

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

.. Now, if the language of the user's locale is French (e.g. ``fr_FR`` or ``fr_BE``),
   the message will be translated into ``J'aime Symfony2``.

ユーザーのロケールがフランス語 (例えば ``fr_FR`` または ``fr_BE``) の時には、
メッセージは ``J'aime Symfony2`` に翻訳されます。

.. The Translation Process
   ~~~~~~~~~~~~~~~~~~~~~~~

翻訳のプロセス
~~~~~~~~~~~~~~

.. To actually translate the message, Symfony2 uses a simple process:

実際にメッセージを翻訳するには、 Symfony2 はシンプルなプロセスで行います。

.. * The ``locale`` of the current user, which is stored in the session, is determined;

* セッションに保存されているユーザーの ``locale`` を見つけ出します。

.. * A catalog of translated messages is loaded from translation resources defined
    for the ``locale`` (e.g. ``fr_FR``). Messages from the fallback locale are
    also loaded and added to the catalog if they don't already exist. The end
    result is a large "dictionary" of translations. See `Message Catalogues`_
    for more details;

* 翻訳済みメッセージのカタログが ``ロケール`` (例えば ``fr_FR``) に定義されている
  翻訳リソースからロードされます。フォールバックロケールからのメッセージも
  同じようにロードされ、まだ存在していない場合にはカタログに追加されます。最終的な
  結果は、翻訳の大きな「辞書」になります。詳しくは `メッセージのカタログ`_ を参照してください。

.. * If the message is located in the catalog, the translation is returned. If
     not, the translator returns the original message.

* メッセージがカタログの中にある場合、翻訳結果が戻り値になります。カタログの中にない場合、
  トランスレータは元のメッセージを返します。

.. When using the ``trans()`` method, Symfony2 looks for the exact string inside
   the appropriate message catalog and returns it (if it exists).

``trans()`` メソッドを使用する時は、 Symfony2 は適切なメッセージカタログの中から
一致する文字列を探し、その文字列を返します (メッセージが存在する場合)。

.. index::
   single: Translations; Message placeholders

.. Message Placeholders
   ~~~~~~~~~~~~~~~~~~~~

メッセージプレースホルダー
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. Sometimes, a message containing a variable needs to be translated:

時によって、メッセージは翻訳の必要がある変数を含んでいることがあります。

.. code-block:: php

    public function indexAction($name)
    {
        $t = $this->get('translator')->trans('Hello '.$name);

        return new Response($t);
    }

.. However, creating a translation for this string is impossible since the translator
   will try to look up the exact message, including the variable portions
   (e.g. "Hello Ryan" or "Hello Fabien"). Instead of writing a translation
   for every possible iteration of the ``$name`` variable, we can replace the
   variable with a "placeholder":

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

.. Symfony2 will now look for a translation of the raw message (``Hello %name%``)
   and *then* replace the placeholders with their values. Creating a translation
   is done just as before:

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

..    The placeholders can take on any form as the full message is reconstructed
      using the PHP `strtr function`_. However, the ``%var%`` notation is
      required when translating in Twig templates, and is overall a sensible
      convention to follow.

.. As we've seen, creating a translation is a two-step process:

ここまで見てきたように、翻訳を作成するには2つのステップがあります。

.. 1. Abstract the message that needs to be translated by processing it through
      the ``Translator``.

1. ``Translator`` を通じて処理を行うことによって、翻訳に必要なメッセージを
   抽出します。

.. 1. Create a translation for the message in each locale that you choose to
     support.

1. サポートしたいロケールごとにメッセージの翻訳を作成します。

.. The second step is done by creating message catalogues that define the translations
   for any number of different locales.

次のステップは異なるロケールに対する翻訳を定義したメッセージのカタログの作成です。

.. index::
   single: Translations; Message catalogues

.. Message Catalogues
   ------------------

メッセージのカタログ
--------------------

.. When a message is translated, Symfony2 compiles a message catalogue for the
   user's locale and looks in it for a translation of the message. A message
   catalogue is like a dictionary of translations for a specific locale. For
   example, the catalogue for the ``fr_FR`` locale might contain the following
   translation:

メッセージが翻訳された時、 Symfony2 はユーザーのロケールに対するメッセージの
カタログをコンパイルし、メッセージの翻訳を探します。メッセージのカタログは、
特定のロケールに対する翻訳の辞書のようなものです。例えば、 ``fr_FR`` ロケールに
対するカタログは、以下のような訳を含んでいます。

    Symfony2 is Great => J'aime Symfony2

.. It's the responsibility of the developer (or translator) of an internationalized
   application to create these translations. Translations are stored on the
   filesystem and discovered by Symfony, thanks to some conventions.

これらの訳を作るのは、国際化されたアプリケーションの開発者 (または翻訳者)
の責任です。翻訳はファイルシステム上に保存され、いくつかの規約の結果、
Symfony に発見されます。

.. index::
   single: Translations; Translation resource locations

.. Translation Locations and Naming Conventions
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

翻訳の場所と名前付け規約
~~~~~~~~~~~~~~~~~~~~~~~~

.. Symfony2 looks for message files (i.e. translations) in two locations:

Symfony2 はメッセージファイル (例として翻訳) を2つの場所から探します。

.. * For messages found in a bundle, the corresponding message files should
    live in the ``Resources/translations/`` directory of the bundle;

* バンドル内で見つけたメッセージに対しては、対応するメッセージファイルは
  バンドルの  ``Resources/translations/`` ディレクトリに存在する必要があります。

.. * To override any bundle translations, place message files in the
    ``app/translations`` directory.

* バンドルの翻訳をオーバーライドするには、メッセージファイルを ``app/translations``
  に置いてください。

.. The filename of the translations is also important as Symfony2 uses a convention
   to determine details about the translations. Each message file must be named
   according to the following pattern: ``domain.locale.loader``:

Symfony2 が翻訳の詳細を理解するのに規約を使用するので、翻訳のファイルネームも重要です。
それぞれのメッセージファイルは、 ``ドメイン.ロケール.ローダー`` というパターンに沿って
いなければなりません。

.. * **domain**: An optional way to organize messages into groups (e.g. ``admin``,
     ``navigation`` or the default ``messages``) - see `Using Message Domains`_;

* **ドメイン**: メッセージをグループに体系づける任意の方法です (例えば ``admin`` 、
  ``navigation`` またはデフォルトの ``messages``)。詳しくは `メッセージドメインの使用`_
  を参照してください。

.. * **locale**: The locale that the translations are for (e.g. ``en_GB``, ``en``, etc);

* **ロケール**: その翻訳のロケールです (例えば ``en_GB`` や ``en`` など)。

.. * **loader**: How Symfony2 should load and parse the file (e.g. ``xml``, ``php``
    or ``yml``).

* **ローダー**: Symfony2 がどのようにファイルをロードし、パースするかです (例えば
  ``xml`` や ``php`` 、 ``yml``)。

.. The loader can be the name of any registered loader. By default, Symfony
   provides the following loaders:

ローダーは、あらゆる登録済みのローダーの名前になり得ます。デフォルトでは、
Symfony は以下のローダーを提供しています。

* ``xml``: XLIFF ファイル
* ``php``:   PHP ファイル
* ``yml``:  YAML ファイル

.. The choice of which loader to use is entirely up to you and is a matter of
   taste.

どのローダーを使用するかは完全にあなた (開発者) 次第で、好みの問題です。

.. note::

    翻訳はデータベースや、 :class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface`
    の実装であるカスタムクラスによって定義されるその他のストレージに保存することも
    できます。どのようにカスタムローダーを登録するかは
    :doc:`Custom Translation Loaders </cookbook/translation/custom_loader>`
    を参照してください (訳注 : 2011/03/13現在、この項は存在していない模様)。

..    You can also store translations in a database, or any other storage by
      providing a custom class implementing the
      :class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface` interface.
      See :doc:`Custom Translation Loaders </cookbook/translation/custom_loader>`
      below to learn how to register custom loaders.

.. index::
   single: Translations; Creating translation resources

.. Creating Translations
   ~~~~~~~~~~~~~~~~~~~~~

翻訳の作成
~~~~~~~~~~

.. Each file consists of a series of id-translation pairs for the given domain and
   locale. The id is the identifier for the individual translation, and can
   be the message in the main locale (e.g. "Symfony is great") of your application
   or a unique identifier (e.g. "symfony2.great" - see the sidebar below):

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

.. Symfony2 will discover these files and use them when translating either
   "Symfony2 is great" or "symfony2.great" into a French language locale (e.g.
   ``fr_FR`` or ``fr_BE``).

Symfony2 はこれらのファイルを見つけ出し、 "Symfony2 is great" や "symfony2.great" 
の両方をフランス語ロケール (``fr_FR`` や ``fr_BE``) に翻訳するのに使います。

.. .. sidebar:: Using Real or Keyword Messages

.. sidebar:: 実際のメッセージあるいはキーワードを使う

    この例では、翻訳されるメッセージを作る時の2つの異なる哲学を表しています。

..    This example illustrates the two different philosophies when creating
      messages to be translated:

    .. code-block:: php

        $t = $translator->trans('Symfony2 is great');

        $t = $translator->trans('symfony2.great');

..    In the first method, messages are written in the language of the default
      locale (English in this case). That message is then used as the "id"
      when creating translations.

    最初の方法では、メッセージはデフォルトロケールで書かれています
    (この場合英語) 。このメッセージは、翻訳を作る際に "id" として使用されます。

..    In the second method, messages are actually "keywords" that convey the
      idea of the message. The keyword message is then used as the "id" for
      any translations. In this case, translations must be made for the default
      locale (i.e. to translate ``symfony2.great`` to ``Symfony2 is great``).

    2番目の方法では、メッセージは実際にはメッセージの意味を伝える「キーワード」に
    なっています。キーワードメッセージはそれからそれぞれの翻訳の「ID」として
    使われます。この場合、翻訳はデフォルトロケール用に作られる必要があります
    (例えば  ``symfony2.great`` は ``Symfony2 is great`` に訳される)。

..    The second method is handy because the message key won't need to be changed
      in every translation file if we decide that the message should actually
      read "Symfony2 is really great" in the default locale.

    デフォルトロケールのメッセージを "Symfony2 is really great" にしたいと考えた
    場合でも、それぞれの翻訳ファイル内のメッセージキーを変更する必要がないことから、 
    2番目の方法は便利です。

..    The choice of which method to use is entirely up to you, but the "keyword"
      format is often recommended. 

    どちらの方法を使うかは完全にあなた次第ですが、「キーワード」フォーマットは
    常に推奨される方法です。

..    Additionally, the ``php`` and ``yaml`` file formats support nested ids to
      avoid repeating yourself if you use keywords instead of real text for your
      ids:

    それに加えて、  ``php`` と ``yaml`` ファイルフォーマットは、 ID に対して
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

..    The multiple levels are flattened into single id/translation pairs by
      adding a dot (.) between every level, therefore the above examples are
      equivalent to the following:

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

.. Using Message Domains
   ---------------------

メッセージドメインの使用
------------------------

.. As we've seen, message files are organized into the different locales that
   they translate. The message files can also be organized further into "domains".
   When creating message files, the domain is the first portion of the filename.
   The default domain is ``messages``. For example, suppose that, for organization,
   translations were split into three different domains: ``messages``, ``admin``
   and ``navigation``. The French translation would have the following message
   files:

これまで見てきたように、メッセージファイルは翻訳されたロケールごとにまとめられます。
また、さらに「ドメイン」ごとにもまとめることができます。メッセージファイルを作成
するさい、ドメインはファイル名の最初の部分になります。デフォルトのドメインは
``messages`` です。例えば、管理上、翻訳が ``messages`` と ``admin`` と ``navigation``
という3つのドメインに分けられていると考えてください。フランス語の翻訳は以下の
メッセージファイルになります。

* ``messages.fr.xml``
* ``admin.fr.xml``
* ``navigation.fr.xml``

.. When translating strings that are not in the default domain (``messages``),
   you must specify the domain as the third argument of ``trans()``:

デフォルトドメイン (``messages``) 内に翻訳文字列がない時には、 ``trans()`` の
3番目の引数としてドメイン名を指定する必要があります。

.. code-block:: php

    $this->get('translator')->trans('Symfony2 is great', array(), 'admin');

.. Symfony2 will now look for the message in the ``admin`` domain of the user's
   locale.

Symfony2 はここでユーザーのロケールの ``admin`` ドメイン内のメッセージを探します。

.. index::
   single: Translations; User's locale

.. Handling the User's Locale
   --------------------------

ユーザーロケールの扱い
----------------------

.. The locale of the current user is stored in the session and is accessible
   via the ``session`` service:

現在のユーザのロケールはセッションに保存され、 ``session`` サービスを介して
アクセスできます。

.. code-block:: php

    $locale = $this->get('session')->getLocale();

    $this->get('session')->setLocale('en_US');

.. index::
   single: Translations; Fallback and default locale

.. Fallback and Default Locale
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~

フォールバックロケールとデフォルトロケール
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. If the locale hasn't been set explicitly in the session, the ``fallback_locale``
   configuration parameter will be used by the ``Translator``. The parameter
   defaults to ``en`` (see `Configuration`_).

セッション内でロケールが明確に指定されていない場合、 ``fallback_locale`` 設定パラメータが
``Translator`` で使用されます。このパラメータのデフォルトは ``en`` です
(詳しくは `設定`_ を参照してください) 。

.. Alternatively, you can guarantee that a locale is set on the user's session
   by defining a ``default_locale`` for the session service:

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

.. The Locale and the URL
   ~~~~~~~~~~~~~~~~~~~~~~

ロケールと URL
~~~~~~~~~~~~~~

.. Since the locale of the user is stored in the session, it may be tempting
   to use the same URL to display a resource in many different languages based
   on the user's locale. For example, ``http://www.example.com/contact`` could
   show content in English for one user and French for another user. Unfortunately,
   this violates a fundamental rule of the Web: that a particular URL returns
   the same resource regardless of the user. To further muddy the problem, which
   version of the content would be indexed by search engines?

ユーザーのロケールはセッション内に保存されるので、ユーザーのロケールに基づいた
色々な言語のリソースを表示するのに、同じ URL が使われることになります。
例えば、 ``http://www.example.com/contact`` はあるユーザーには英語で、別なユーザーには
フランス語で表示されます。残念ながら、これは Web の基本的なルール、すなわち、
ある URL はユーザーに関係なく同じリソースを返す、というルールに反しています。
さらに問題がややこしくなるのが、どのバージョンのコンテンツが検索エンジンでインデックス
されるのか？ということです。

.. A better policy is to include the locale in the URL. This is fully-supported
   by the routing system using the special ``_locale`` parameter:

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

.. When using the special `_locale` parameter in a route, the matched locale
   will *automatically be set on the user's session*. In other words, if a user
   visits the URI ``/fr/contact``, the locale ``fr`` will automatically be set
   as the locale for the user's session.

ルートの中で特別な `_locale` パラメータを使用する際、一致するロケールが
*自動的にユーザーセッションに設定されます* 。言い換えると、ユーザーが
``/fr/contact`` という URI を訪れると、 ``fr`` というロケールが自動的に
ユーザーのセッションのロケールとして設定されます。

.. You can now use the user's locale to create routes to other translated pages
   in your application.

これで、ユーザーのロケールをアプリケーション内の他の翻訳されたページへのルートを
作るのに使用できるようになります。

.. index::
   single: Translations; Pluralization

.. Pluralization
   -------------

複数型への対応
--------------

.. Message pluralization is a tough topic as the rules can be quite complex. For
   instance, here is the mathematic representation of the Russian pluralization
   rules::

メッセージの複数型への対応は、ルールがなかなか複雑であるため、大きな問題です。
例えば、これはロシア語の複数型の数学的表現です。::

    (($number % 10 == 1) && ($number % 100 != 11)) ? 0 : ((($number % 10 >= 2) && ($number % 10 <= 4) && (($number % 100 < 10) || ($number % 100 >= 20))) ? 1 : 2);

.. As you can see, in Russian, you can have three different plural forms, each
   given an index of 0, 1 or 2. For each form, the plural is different, and
   so the translation is also different.

見ての通り、ロシア語では、それぞれ0、1あるいは2のインデックスを与えられた3つの異なる
複数型の表現があります。それぞれの表現で複数形は異なりますので、翻訳も同様に異なります。

.. When a translation has different forms due to pluralization, you can provide
   all the forms as a string separated by a pipe (``|``)::

複数型への対応のために翻訳の表現が異なる時、それら全ての表現をパイプ (``|``) で
区切られた文字列として与えることができます。::

    'There is one apple|There are %count% apples'

.. To translate pluralized messages, use the
   :method:`Symfony\\Component\\Translation\\Translator::transChoice` method:

複数型に対応したメッセージを翻訳するため、 :method:`Symfony\\Component\\Translation\\Translator::transChoice` メソッドを使用できます。

.. code-block:: php

    $t = $this->get('translator')->transChoice(
        'There is one apple|There are %count% apples',
        10,
        array('%count%' => 10)
    );

.. The second argument (``10`` in this example), is the *number* of objects being
   described and is used to determine which translation to use and also to populate
   the ``%count%`` placeholder.

2つ目の引数 (この例では ``10``) は記述されるオブジェクトの *数* であり、
どの翻訳が使われるかを決めるのに使われ、 ``%count%`` プレースホルダーに
投入されます。

.. Based on the given number, the translator chooses the right plural form.
   In English, most words have a singular form when there is exactly one object
   and a plural form for all other numbers (0, 2, 3...). So, if ``count`` is
   ``1``, the translator will use the first string (``There is one apple``)
   as the translation. Otherwise it will use ``There are %count% apples``.

与えられた数字に従い、トランスレータは適切な複数型の表現を選びます。
英語の場合、多くの単語はぴったり1つしかオブジェクトがない時には
単数形で、それ以外の数 (0, 2, 3...) の時には複数型になります。
従って、 ``count`` が ``1`` の時には、トランスレータ―は最初の文字列
(``There is one apple``) を翻訳として使い、そうでない場合は
``There are %count% apples`` を使用します。

.. Here is the French translation::

以下はフランス語の翻訳です。::

    'Il y a %count% pomme|Il y a %count% pommes'

.. Even if the string looks similar (it is made of two sub-strings separated by a
   pipe), the French rules are different: the first form (no plural) is used when
   ``count`` is ``0`` or ``1``. So, the translator will automatically use the
   first string (``Il y a %count% pomme``) when ``count`` is ``0`` or ``1``.

文字列は同じように見えたとしても (パイプで区切られた2つの部分文字列からなっています)、
フランス語の表現は異なります。最初の表現 (複数形ではない) は ``count`` が ``0`` か
``1`` の時に使われます。従って、トランスレータは ``count`` が ``0`` または ``1``
の時には自動的に最初の文字列 (``Il y a %count% pomme``) を使います。

.. Each locale has its own set of rules, with some having as many as six different
   plural forms with complex rules behind which numbers map to which plural form.
   The rules are quite simple for English and French, but for Russian, you'd
   may want a hint to know which rule matches which string. To help translators,
   you can optionally "tag" each string::

それぞれのロケールは独自の表現のセットを持っています。いくつかのロケールは、どの数字が
どの複数形にマップされるかの複雑なルールがある、6つの異なる複数形の表現を持っています。
英語とフランス語のルールはかなりシンプルですが、ロシア語では、どの表現がどの文字列に
一致するのか知るためにヒントが欲しくなるでしょう。翻訳者を手助けするために、
オプションとしてそれぞれの文字列に対して「タグをつける」ことができます。::

    'one: There is one apple|some: There are %count% apples'

    'none_or_one: Il y a %count% pomme|some: Il y a %count% pommes'

.. The tags are really only hints for translators and don't affect the logic
   used to determine which plural form to use. The tags can be any descriptive
   string that ends with a colon (``:``). The tags also do not need to be the
   same in the original message as in the translated one.

タグは翻訳者のためのヒントでしかありませんので、どの複数形の表現を使うか決める
ロジックには影響しません。タグはコロン (``:``) で終わる説明を含む文字列になります。
また、タグが翻訳された元のメッセージと同じである必要はありません。

.. tip:

..    As tags are optional, the translator doesn't use them (the translator will
      only get a string based on its position in the string).

    タグはオプションですので、トランスレータはタグを使用しません (トランスレータは
    単純に文字列内の位置に応じた文字列を取得するだけです) 。

.. Explicit Interval Pluralization
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

間隔を明示した複数型への対応
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. The easiest way to pluralize a message is to let Symfony2 use internal logic
   to choose which string to use based on a given number. Sometimes, you'll
   need more control or want a different translation for specific cases (for
   ``0``, or when the count is negative, for example). For such cases, you can
   use explicit math intervals::

メッセージを複数形にする最も簡単な方法は、与えられた数を元にどの文字列を選ぶかの
Symfony2 の内部ロジックを使うことです。場合によっては、もっと翻訳の動作を自由に
制御したかったり、違う訳が欲しい時があるでしょう (例えば ``0`` に関してや、負の数の時)。
このような場合、明示的な数の間隔を使用できます。::

    '{0} There is no apples|{1} There is one apple|]1,19] There are %count% apples|[20,Inf] There are many apples'

.. The intervals follow the `ISO 31-11`_ notation. The above string specifies
   four different intervals: exactly ``0``, exactly ``1``, ``2-19``, and ``20``
   and higher.

間隔は `ISO 31-11`_ 規格に従っています。上の文字列は 4 つの異なる間隔を定義しています。
ちょうど ``0`` 、ちょうど ``1`` 、 ``2 から 19`` 、 ``20`` 以上、です。

.. You can also mix explicit math rules and standard rules. In this case, if
   the count is not matched by a specific interval, the standard rules take
   effect after removing the explicit rules::

明示的な数の表現と、標準の表現を混在することもできます。この場合、数が指定された
間隔と一致しない時には、明示的な表現が削除された後に標準の表現が有効になります。::

    '{0} There is no apples|[20,Inf] There are many apples|There is one apple|a_few: There are %count% apples'

.. For example, for ``1`` apple, the standard rule ``There is one apple`` will
   be used. For ``2-19`` apples, the second standard rule ``There are %count%
   apples`` will be selected.

例えば、 ``1`` つのリンゴの場合、標準の表現 ``There is one apple`` が使われます。
``2から19`` 個のリンゴの場合、2番目の標準の表現である ``There are %count%
apples`` が使われます。

.. An :class:`Symfony\\Component\\Translation\\Interval` can represent a finite set
   of numbers::

:class:`Symfony\\Component\\Translation\\Interval` クラスで数の有限集合を表せます。::

    {1,2,3,4}

.. Or numbers between two other numbers::

または 2 と他の数の間なら以下のようになります。::

    [1, +Inf[
    ]-1,2[

.. The left delimiter can be ``[`` (inclusive) or ``]`` (exclusive). The right
   delimiter can be ``[`` (exclusive) or ``]`` (inclusive). Beside numbers, you
   can use ``-Inf`` and ``+Inf`` for the infinite.

左側のデリミターは ``[`` (含む) または ``]`` (含まない) になります。
右側のデリミターは  ``[`` (含まない) または ``]`` (含む) です。
数と合わせて、 ``-Inf`` と ``+Inf`` を無限を表すのに使用できます。

.. index::
   single: Translations; In templates

.. Translations in Templates
   -------------------------

テンプレート内の翻訳

.. Most of the time, translation occurs in templates. Symfony2 provides native
   support for both Twig and PHP templates.

多くの場合、翻訳はテンプレート内で発生します。 Symfony2 は Twig と PHP テンプレートの
両方をネイティブでサポートします。

.. Twig Templates
   ~~~~~~~~~~~~~~

Twig テンプレート
~~~~~~~~~~~~~~~~~

.. Symfony2 provides specialized Twig tags (``trans`` and ``transChoice``) to help
   with message translation:

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

.. The ``transChoice`` tag automatically gets the ``%count%`` variable from
   the current context and passes it to the translator. This mechanism only
   works when you use a placeholder following the ``%var%`` pattern.

``transChoice`` タグは自動的に現在のコンテキストから ``%count%`` 変数を取り出し、
トランスレータに渡します。このメカニズムは ``%var%`` というパターンに従った
プレースホルダーを使用した場合にのみ動作します。

.. You can also specify the message domain:

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

.. PHP Templates
   ~~~~~~~~~~~~~

PHP テンプレート
~~~~~~~~~~~~~~~~

.. The translator service is accessible in PHP templates through the
   ``translator`` helper:

トランスレータサービスへは、 ``translator`` ヘルパーを通じて PHP テンプレートからも
アクセスできます。

.. code-block:: html+php

    <?php echo $view['translator']->trans('Symfony2 is great') ?>

    <?php echo $view['translator']->transChoice(
        '{0} There is no apples|{1} There is one apple|]1,Inf[ There are %count% apples',
        10,
        array('%count%' => 10)
    ) ?>

.. Forcing Translation Locale
   --------------------------

翻訳ロケールの強制
------------------

.. When translating a message, Symfony2 uses the locale from the user's session
   or the ``fallback`` locale if necessary. You can also manually specify the
   locale to use for translation:

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

.. Translating Database Content
   ----------------------------

データベースコンテンツの翻訳
----------------------------

.. The translation of database content should be handled by Doctrine through
   the `Translatable Extension`_. For more information, see the documentation
   for that library.

データベースコンテンツの翻訳は `Translatable Extension`_ を通じて Doctrine
によって扱われるべきです。詳しくは、ライブラリのドキュメントを参照してください。

.. Summary
   -------

概要
----

.. With the Symfony2 Translation component, creating an internationalized application
   no longer needs to be a painful process and boils down to just a few basic
   steps:

Symfony2 の Translation コンポーネントを使用すると、国際化されたアプリケーションを
作ることはもはや苦痛なプロセスではなく、いくつかの基本的なステップに要約する
ことができます。

.. * Abstract messages in your application by wrapping each in either the
     :method:`Symfony\\Component\\Translation\\Translator::trans` or
     :method:`Symfony\\Component\\Translation\\Translator::transChoice` methods;

:method:`Symfony\\Component\\Translation\\Translator::trans` メソッドまたは
:method:`Symfony\\Component\\Translation\\Translator::transChoice` メソッドの
いずれかでラッピングすることによって、アプリケーション内のメッセージを抽象化します。

.. * Translate each message into multiple locales by creating translation message
     files. Symfony2 discovers and processes each file because its name follows
     a specific convention;

それぞれのメッセージを、翻訳メッセージファイルを作成することで複数のロケールに
翻訳します。 メッセージファイルの名前は指定された規約に則っており、Symfony2 は
それぞれのファイルを見つけ出して処理を行います。

.. * Manage the user's locale, which is stored in the session.

セッションに保存されているユーザーのロケールを管理します。

.. _`strtr 関数`: http://www.php.net/manual/en/function.strtr.php
.. _`ISO 31-11`: http://en.wikipedia.org/wiki/Interval_%28mathematics%29#The_ISO_notation
.. _`Translatable Extension`: https://github.com/l3pp4rd/DoctrineExtensions