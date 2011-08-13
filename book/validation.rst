.. index::
   single: Validation

バリデーション
==============

バリデーションは、WEB アプリケーションでとてもよく使われるタスクです。\
フォームに入力されたデータは、バリデートする必要があります。\
また、データベースに書かれる前や、WEB サービスに渡す前にもバリデーションが必要です。

Symfony2 には `Validator`_ コンポーネントが同梱されており、このタスクをとても簡単に、そして透過的にこなすことができます。\
このコンポーネントは、\ `JSR303 Bean Validation specification`_ が元になっています。\
え？PHP で Java の仕様だって？\
聞き間違いではありませんよ。別に言うほどおかしいことでもないんです。\
それでは、PHP でどうやってこれを使うのか見ていきましょう。


.. index:
   single: Validation; The basics

バリデーションの基礎
--------------------

バリデーションを理解するのに最も良い方法は、実際に動作しているのを見ていくことでしょう。\
まず、アプリケーションのどこかで使用する単純な PHP クラスを作ったとしましょう。

.. code-block:: php

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    class Author
    {
        public $name;
    }

今のところ、このクラスは至って普通のクラスで、アプリケーション内で何かするためのクラスでしょう。\
バリデーションの目的は、オブジェクトのデータが妥当であるかどうかを示す、ということです。\
このためには、オブジェクトが妥当であるために従うべきルールのリスト(:ref:`制約<validation-constraints>` と呼ぶ)を設定しなければなりません。\
このルールは、様々なフォーマット(YAML、XML、アノテーション、PHP)で指定することができます。

例えば、\ ``$name`` プロパティが空で無い事を保証するには、次のようにします。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                name:
                    - NotBlank: ~

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/services/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="name">
                    <constraint name="NotBlank" />
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank()
             */
            public $name;
        }

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;

        class Author
        {
            public $name;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('name', new NotBlank());
            }
        }
.. ** <- vimのハイライトの問題。無視してください。

.. tip::

    protected / private なプロパティでもバリデートすることが可能です。\
    また、「ゲッター」メソッドに対してもバリデートが可能です(:ref:`validator-constraint-targets` 参照)。

.. index::
   single: Validation; Using the validator

``validator`` サービスを使用する
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

続いて ``Author`` オブジェクトを実際にバリデートしてみます。これは ``validator`` サービス(:class:`Symfony\\Component\\Validator\\Validator`)の ``validate`` メソッドを使用します。\
``validator`` が行う仕事は簡単なものです。\
クラスの制約(ルール)を読み込んで、そのオブジェクトのデータがその制約を満たしているかどうか、を検証することです。\
バリデーションが失敗すれば、エラー配列が返ってきます。\
では、単純なコントローラの例を見てみましょう。

.. code-block:: php

    use Symfony\Component\HttpFoundation\Response;
    use Acme\BlogBundle\Entity\Author;
    // ...

    public function indexAction()
    {
        $author = new Author();
        // ... do something to the $author object

        $validator = $this->get('validator');
        $errors = $validator->validate($author);

        if (count($errors) > 0) {
            return new Response(print_r($errors, true));
        } else {
            return new Response('The author is valid! Yes!');
        }
    }

``$name`` プロパティが空であれば、次のようなエラーを見てしまうことになるでしょう。

.. code-block:: text

    Acme\BlogBundle\Author.name:
        This value should not be blank

``name`` プロパティに値を入れると、成功メッセージが見えるはずです。

.. tip::

    ``validator`` サービスと直接やり取りしてエラー出力の心配しなくてはならない、といった事態には、まずならないでしょう。\
    大抵の場合は、フォームデータを扱うときに間接的にバリデーションを使用することになります。\
    詳しくは、\ :ref:`book-validation-forms` を見てください。

エラーコレクションをテンプレートに渡すこともできます。

.. code-block:: php

    if (count($errors) > 0) {
        return $this->render('AcmeBlogBundle:Author:validate.html.twig', array(
            'errors' => $errors,
        ));
    } else {
        // ...
    }

テンプレート内では、必要に応じてエラーリストを出力することができます。

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/Author/validate.html.twig #}

        <h3>The author has the following errors</h3>
        <ul>
        {% for error in errors %}
            <li>{{ error.message }}</li>
        {% endfor %}
        </ul>

    .. code-block:: html+php

        <!-- src/Acme/BlogBundle/Resources/views/Author/validate.html.php -->

        <h3>The author has the following errors</h3>
        <ul>
        <?php foreach ($errors as $error): ?>
            <li><?php echo $error->getMessage() ?></li>
        <?php endforeach; ?>
        </ul>

.. note::

    各バリデーションエラー(「制約違反」と呼ぶ) は、\ :class:`Symfony\\Component\\Validator\\ConstraintViolation` オブジェクトで表現されています。

.. index::
   single: Validation; Validation with forms

.. _book-validation-forms:

バリデーションとフォーム
~~~~~~~~~~~~~~~~~~~~~~~~

``validator`` サービスは、いつでも、どんなオブジェクトをバリデートする場合でも使用することができます。\
しかし、実際は、フォームと連携する場合に ``validator`` と間接的にやり取りすることが普通でしょう。\
Symfony のフォームライブラリは、フォームがサブミット・バインドされた後のオブジェクトの値をバリデートする際に、\
この ``validator`` サービスを内部的に使用しています。\
オブジェクトの制約違反は、 ``FieldError`` オブジェクトに変換され、フォームと共に簡単に表示できるようになります。\
典型的なフォームサブミットのワークフローは、次のようなコントローラになります。\ ::


    use Acme\BlogBundle\Entity\Author;
    use Acme\BlogBundle\Form\AuthorType;
    use Symfony\Component\HttpFoundation\Request;
    // ...

    public function updateAction(Request $request)
    {
        $author = new Acme\BlogBundle\Entity\Author();
        $form = $this->createForm(new AuthorType(), $author);

        if ($request->getMethod() == 'POST') {
            $form->bindRequest($request);

            if ($form->isValid()) {
                // the validation passed, do something with the $author object

                $this->redirect($this->generateUrl('...'));
            }
        }

        return $this->render('BlogBundle:Author:form.html.twig', array(
            'form' => $form->createView(),
        ));
    }

.. note::

    この例では、\ ``AuthorType`` フォームクラスを使用しています。ここでは割愛しています。

詳細は :doc:`フォーム</book/forms>` 章を参照してください。

.. index::
   pair: Validation; Configuration

.. _book-validation-configuration:

設定
----

Symfony2 のバリデータはデフォルトで有効になっていますが、\
アノテーション方式で制約を指定している場合は、明示的に有効にする必要があります。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            validation: { enable_annotations: true }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:validation enable_annotations="true" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array('validation' => array(
            'enable_annotations' => true,
        )));

.. index::
   single: Validation; Constraints

.. _validation-constraints:

制約
----

``validator`` は、オブジェクトを *constraints(制約)*\ (ルール)に対してバリデートするように設計されています。\
オブジェクトをバリデートするときは、単に一つ以上の制約をクラスにマッピングし、それを ``validator`` サービスに渡します。

..  制約の訳し方がちょっと曖昧。。

この裏側ですが、制約自体は、アサーティブなステイトメントを作るシンプルな PHP オブジェクトとなっています。\
現実の世界では、制約は「ケーキは焦がしてはいけない」という風になるでしょう。\
Symfony2 でも同様で、条件が真になる、というアサーションです。\
制約は、与えられた値が制約ルールを守っているかどうかということを教えてくれます。

サポートされている制約
~~~~~~~~~~~~~~~~~~~~~~

Symfony2 は一般的に必要となる制約を多数パッケージしています。\
すべての制約のリストは、\ :doc:`constraints reference section</reference/constraints>` で参照可能です。


.. index::
   single: Validation; Constraints configuration

.. _book-validation-constraint-configuration:

制約の設定
~~~~~~~~~~

:doc:`NotBlank</reference/constraints/NotBlank>` のようなシンプルな制約もあれば、\
:doc:`Choice</reference/constraints/Choice>` のように複数の設定オプションが存在する制約もあります。\
``Auhtor`` クラスが、例えば ``gender`` という "male" もしくは "female" の値が設定されるプロパティを持っていたとしましょう。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice: { choices: [male, female], message: Choose a valid gender. }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/services/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="gender">
                    <constraint name="Choice">
                        <option name="choices">
                            <value>male</value>
                            <value>female</value>
                        </option>
                        <option name="message">Choose a valid gender.</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice(
             *     choices = { "male", "female" },
             *     message = "Choose a valid gender."
             * )
             */
            public $gender;
        }

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;

        class Author
        {
            public $gender;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Choice(array(
                    'choices' => array('male', 'female'),
                    'message' => 'Choose a valid gender.',
                )));
            }
        }

.. ** <- vimのハイライトの問題。無視してください。

制約のオプションは、常に配列で渡されますが、\
いくつかの制約では、その配列の場所に *default* オプションの値だけを渡すことも可能です。
``Choice`` 制約の場合であれば、\ ``choices`` オプションを次のように指定できます。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice: [male, female]

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/services/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="gender">
                    <constraint name="Choice">
                        <value>male</value>
                        <value>female</value>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice({"male", "female"})
             */
            protected $gender;
        }

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Choice;

        class Author
        {
            protected $gender;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Choice(array('male', 'female')));
            }
        }

.. ** <- vimのハイライトの問題。無視してください。

この方法は純粋に、制約の最も一般的なオプションの設定を、短くそして素早く書くことができるようにするという意図からです。

オプションの指定の仕方がわからない場合は、制約の API ドキュメントを確認するか、\
オプション配列を常に渡せば安全です(上述した最初の方法)。


.. index::
   single: Validation; Constraint targets

.. _validator-constraint-targets:

制約の対象
----------

制約は、クラスのプロパティ(例えば ``name``) や、\
public なゲッターメソッド(例えば ``getFullName``) にも適用することができます。\
前者は最も一般的で簡単ですが、後者の場合は、より複雑なバリデーションルールを指定することができます。

.. index::
   single: Validation; Property constraints

プロパティ
~~~~~~~~~~

クラスのプロパティをバリデートすること。これが最も基本的なバリデーションのテクニックです。\
Symfony2 では、private や protected、public なプロパティをバリデートすることができます。\
次の例では、\ ``Author`` クラスの ``$firstName`` プロパティが、3文字以上であること、という設定を示しています。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                firstName:
                    - NotBlank: ~
                    - MinLength: 3

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="firstName">
                <constraint name="NotBlank" />
                <constraint name="MinLength">3</constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank()
             * @Assert\MinLength(3)
             */
            private $firstName;
        }

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\MinLength;

        class Author
        {
            private $firstName;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('firstName', new NotBlank());
                $metadata->addPropertyConstraint('firstName', new MinLength(3));
            }
        }

.. ** <- vimのハイライトの問題。無視してください。

.. index::
   single: Validation; Getter constraints

ゲッター
~~~~~~~~

制約は、メソッドの返り値に対しても適用することができます。\
Symfony2 では、"get" や "is" で始まる public なメソッドであれば、\
それに対して制約を追加することができます。\
このガイドでは、両メソッドを総称して「ゲッター」と呼ぶことにします。

このやり方では、オブジェクトを動的にバリデートできるようになる、という恩恵を受けることができます。\
例えば、パスワードフィールドが、(セキュリティ的な理由で)そのユーザのファーストネームと一致してはいけない、という状況を考えてみましょう。\
これは、\ ``isPasswordLegal`` というメソッドを作成し、\
そのメソッドが ``true`` を返さなければならない、というアサーションを行うことで可能になります。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            getters:
                passwordLegal:
                    - True: { message: "The password cannot match your first name" }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <getter property="passwordLegal">
                <constraint name="True">
                    <option name="message">The password cannot match your first name</option>
                </constraint>
            </getter>
        </class>

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\True(message = "The password cannot match your first name")
             */
            public function isPasswordLegal()
            {
                // return true or false
            }
        }

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\True;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addGetterConstraint('passwordLegal', new True(array(
                    'message' => 'The password cannot match your first name',
                )));
            }
        }

.. ** <- vimのハイライトの問題。無視してください。

そして ``isPasswordLegal()`` メソッドを作成し、必要なロジックをそこに盛り込みます。\ ::

    public function isPasswordLegal()
    {
        return ($this->firstName != $this->password);
    }

.. note::

    .. todo: 訳違うかも。。

    鋭い目をお持ちのあなたでしたら、マッピング情報内でゲッターのプリフィックス("get" や "is")が省略されていることに気づくでしょう。\
    こうしておくことで、後々同じ名前で、ロジックを変更すること無く制約をプロパティに移動させる(もしくはその逆も)ことができます。
   

.. _book-validation-validation-groups:

バリデーショングループ
----------------------

ここまでで、クラスに対して制約を追加し、その定義した制約が通るかどうか問い合わせすることが可能になりました。\
とはいえ、そのクラスの\ *いくつかの*\ 制約に対してだけバリデートしたいという場合も往々にしてあるでしょう。\
こう言った場合は、1つ以上の「バリデーショングループ」を作り、\
ある1つの制約グループに対してだけバリデーションを適用することができます。

たとえば、\ ``User`` クラスを考えてみましょう。\
このクラスは、ユーザーの登録時と、それ以後にユーザが連絡先情報を更新する場合に使われるとします。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\User:
            properties:
                email:
                    - Email: { groups: [registration] }
                password:
                    - NotBlank: { groups: [registration] }
                    - MinLength: { limit: 7, groups: [registration] }
                city:
                    - MinLength: 2

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\User">
            <property name="email">
                <constraint name="Email">
                    <option name="groups">
                        <value>registration</value>
                    </option>
                </constraint>
            </property>
            <property name="password">
                <constraint name="NotBlank">
                    <option name="groups">
                        <value>registration</value>
                    </option>
                </constraint>
                <constraint name="MinLength">
                    <option name="limit">7</option>
                    <option name="groups">
                        <value>registration</value>
                    </option>
                </constraint>
            </property>
            <property name="city">
                <constraint name="MinLength">7</constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/User.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Security\Core\User\UserInterface
        use Symfony\Component\Validator\Constraints as Assert;

        class User implements UserInterface
        {
            /**
            * @Assert\Email(groups={"registration"})
            */
            private $email;

            /**
            * @Assert\NotBlank(groups={"registration"})
            * @Assert\MinLength(limit=7, groups={"registration"})
            */
            private $password;

            /**
            * @Assert\MinLength(2)
            */
            private $city;
        }

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/User.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Email;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\MinLength;

        class User
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('email', new Email(array(
                    'groups' => array('registration')
                )));

                $metadata->addPropertyConstraint('password', new NotBlank(array(
                    'groups' => array('registration')
                )));
                $metadata->addPropertyConstraint('password', new MinLength(array(
                    'limit'  => 7,
                    'groups' => array('registration')
                )));

                $metadata->addPropertyConstraint('city', new MinLength(3));
            }
        }

.. ** <- vimのハイライトの問題。無視してください。

上記のように設定した場合は、2つのバリデーショングループが存在することになります。

* ``Default`` - どのグループにも属していない制約群

* ``registration`` - ``email`` と ``password`` フィールドのみに対する制約群

バリデータに特定のグループのみを使用するように命令するには、\ ``validate()`` メソッドの第二引数に\
1つ以上のグループ名を渡します。\ ::

    $errors = $validator->validate($author, array('registration'));

通常はフォームライブラリを通して間接的にバリデーションを扱います。\
フォーム内でのバリデーショングループの使用に関しては、\ :ref:`book-forms-validation-groups` を参照してください。



Final Thoughts
--------------

Symfony2 の ``validator`` は、あらゆるオブジェクトのデータに対して「valid(妥当)」かどうかを保証するために活用できる、強力なツールです。\
バリデーションの動力は「constraints(制約)」にあり、制約はオブジェクトのプロパティやゲッターメソッドに対して適用することができます。\
ほとんどの場合は、フォームの使用を通して間接的にバリデーションフレームワークを使用することにはなりますが、\
あらゆるオブジェクトに対してあらゆる場所でこのバリデーションが使用できることは覚えておいてください。

クックブックでより深く
----------------------

* :doc:`/cookbook/validation/custom_constraint`

.. _Validator: https://github.com/symfony/Validator
.. _JSR303 Bean Validation specification: http://jcp.org/en/jsr/detail?id=303

.. 2011/08/30 gilbite a348935
